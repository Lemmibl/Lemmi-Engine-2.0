#pragma once
#include <glew.h>
#include <GLFW/glfw3.h>
#include <assimp/scene.h>
#include "../Objects/Texture.h"

#include "HandlerBaseClass.h"

class TextureHandler : HandlerBaseClass<Texture, unsigned short>
{
public:
	TextureHandler();
	~TextureHandler();

	bool LoadTexture(std::string filename, FWHandle& outHandle);
	bool LoadTextureAssimp(aiMaterial* material, std::string filename, unsigned int textureIndex, FWHandle& outHandle);	
	bool GetTexture(FWHandle handle, Texture& outTexture);

private:
	FWHandle placeHolderTexture;
};

