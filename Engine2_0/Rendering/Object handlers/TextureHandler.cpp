#include "TextureHandler.h"

#include <SOIL2.h>
#include <stb_image.h>
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
	if(AI_SUCCESS == material->GetTexture(aiTextureType_DIFFUSE, textureIndex, &texname, NULL, NULL, NULL, NULL, NULL)) 
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

		int width, height, channels;

		unsigned char* data = SOIL_load_image(fullPath.c_str(), &width, &height, &channels, SOIL_LOAD_AUTO);

		unsigned int oglTexHandle = 0;

		if(data)
		{
			oglTexHandle = SOIL_create_OGL_texture(data, &width, &height, channels, SOIL_CREATE_NEW_ID, SOIL_FLAG_TEXTURE_REPEATS|SOIL_FLAG_GL_MIPMAPS);
		}

		SOIL_free_image_data( data );


		if(0 != oglTexHandle)
		{
			glBindTexture(GL_TEXTURE_2D, oglTexHandle);
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

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
	}

	return false;
}


bool TextureHandler::GetTexture(FWHandle handle, Texture& outTexture)
{
	unsigned short key = FlyweightFunctionality::GetKey(handle);

	//Look for the object
	if(objectContainer.IsValid(key))
	{
		outTexture = objectContainer[key];

		return true;
	}

	//Key isn't valid
	return false;
}
