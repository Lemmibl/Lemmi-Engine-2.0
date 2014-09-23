#pragma once
#include <glew.h>
#include <GLFW/glfw3.h>
#include <assimp/scene.h>

#include "HandlerBaseClass.h"

class TextureHandler : HandlerBaseClass<GLuint, unsigned short>
{
public:
	TextureHandler();
	~TextureHandler();

	GLuint GetTexture(FlyweightHandle handle);
	FlyweightHandle LoadTexture(std::string filename, aiMaterial* material);
};

