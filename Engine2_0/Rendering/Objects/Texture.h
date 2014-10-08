#pragma once
#include <glew.h>
#include <GLFW/glfw3.h>

class Texture
{
public:
	Texture();
	~Texture();

	void Initialize(GLenum textureType, GLuint textureID);

	GLuint GetTextureID() { return textureID; }

	//Input parameter defines what slot we'll bind the tex to. For example: GL_TEXTURE0
	void Bind(GLenum textureSlot, GLint shaderTextureIndex);

private:
	GLuint textureID;
	GLenum textureType;
};

