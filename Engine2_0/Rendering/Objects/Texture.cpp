#include "Texture.h"


Texture::Texture()
{
}


Texture::~Texture()
{
}

void Texture::Initialize(GLenum texType, GLuint texID)
{
	textureType = texType;
	textureID = texID;
}


void Texture::Bind(GLenum textureSlot, GLint shaderTextureIndex)
{
	glActiveTexture(textureSlot);
	glBindTexture(textureType, textureID);
	glUniform1i(shaderTextureIndex, textureSlot);
}
