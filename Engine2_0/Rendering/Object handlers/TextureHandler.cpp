#include "TextureHandler.h"

#include <SOIL2.h>
#include <easylogging++.h>

static const std::string baseFilePath = "../Engine2_0/Data/Textures/";

TextureHandler::TextureHandler()
	: HandlerBaseClass(64)
{	
	if(!LoadTexture("placeholder.tga", placeHolderTexture))
	{
			LOG(ERROR) << "Couldn't load placeholder texture in TextureHandler constructor.";
	}
}


TextureHandler::~TextureHandler()
{
}

bool TextureHandler::LoadTextureAssimp(aiMaterial* material, std::string filename, unsigned int textureIndex, FWHandle& outHandle)
{
	aiString texname;

	//If all else fails we'll just return placeholder texture.
	outHandle = placeHolderTexture;

	//See if we can fetch a diffuse texture from the material
	if(material->GetTexture(aiTextureType_DIFFUSE, textureIndex, &texname, NULL, NULL, NULL, NULL, NULL) == AI_SUCCESS) 
	{
		std::string textureName(texname.C_Str());

		//Use this function instead
		return LoadTexture(textureName, outHandle);
	}

	return false;
}

bool TextureHandler::LoadTexture( std::string filename, FWHandle& outHandle )
{
	if(LookForDuplicateObject(filename, outHandle))
	{
		return true;
	}

	unsigned short newTextureHandle;

	if(objectContainer.AddNewObject(newTextureHandle))
	{
		std::string fullPath = baseFilePath + filename;

		//Load texture and automatically store it inside openGL's ... innards.
		GLuint oglTexHandle = SOIL_load_OGL_texture
		(
			fullPath.c_str(),
			SOIL_LOAD_AUTO,
			SOIL_CREATE_NEW_ID,
			SOIL_FLAG_TEXTURE_REPEATS|SOIL_FLAG_GL_MIPMAPS
		);

		glBindTexture(GL_TEXTURE_2D, oglTexHandle);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		Texture newTexture;

		//I need to use initialize function instead of constructor because my container class doesn't handle things with no default constructors
		newTexture.Initialize(GL_TEXTURE_2D, oglTexHandle);

		//Save it in our container.			//Create a texture object. Very thin wrapper around it all.
		objectContainer[newTextureHandle] = newTexture;

		//Make sure we encode the handle properly
		outHandle = CreateHandle(HandleTypes::Texture, newTextureHandle);

		InsertNewPair(filename, outHandle);

		return true;
	}

	return false;
}


bool TextureHandler::GetTexture(FWHandle handle, Texture& outTexture)
{
	unsigned int key = FlyweightFunctionality::GetKey(handle);

	//Look for the object
	if(objectContainer.IsValid(key))
	{
		outTexture = objectContainer[key];

		return true;
	}

	//Key isn't valid
	return false;
}
