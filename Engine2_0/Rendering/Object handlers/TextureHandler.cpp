#include "TextureHandler.h"

#include <SOIL2.h>

static const std::string baseFilePath = "../Engine2_0/Data/Textures/";

TextureHandler::TextureHandler()
: HandlerBaseClass(64)
{
}


TextureHandler::~TextureHandler()
{
}

FlyweightHandleFunctions::FlyweightHandle TextureHandler::LoadTexture(std::string filename, aiMaterial* material)
{
	FlyweightHandle returnHandle = 0;

	if(LookForDuplicateObject(filename, returnHandle))
	{
		return returnHandle;
	}

	unsigned short newTextureHandle;

	if(objectContainer.AddNewObject(newTextureHandle))
	{
		objectContainer[newTextureHandle] = NULL;

		//If the material contains any diffuse textures
		if(material->GetTextureCount(aiTextureType_DIFFUSE) > 0)
		{
			aiString textureName;

			//See if we can fetch a diffuse texture from the material
			if(material->GetTexture(aiTextureType_DIFFUSE, 0, &textureName, NULL, NULL, NULL, NULL, NULL) == AI_SUCCESS) 
			{
				std::string fullPath = baseFilePath + textureName.C_Str();

				//Load texture and automatically store it inside openGL's ... innards.
				GLuint oglTexHandle = SOIL_load_OGL_texture
				(
					fullPath.c_str(),
					SOIL_LOAD_AUTO,
					SOIL_CREATE_NEW_ID,
					0 //No flags right now
				);

				//Save it in our container.
				objectContainer[newTextureHandle] = oglTexHandle;

				//Make sure we encode the handle properly
				returnHandle = CreateHandle(HandleTypes::Texture, newTextureHandle);

				InsertNewPair(filename, returnHandle);
			}
		}
	}

	return returnHandle;
}