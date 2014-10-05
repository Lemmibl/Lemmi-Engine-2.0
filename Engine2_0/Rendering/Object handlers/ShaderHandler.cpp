#include "ShaderHandler.h"

#include <iostream>
#include <fstream>
#include <string>

#include "../Shader code/OBJShader.h"

ShaderHandler::ShaderHandler()
{
}


ShaderHandler::~ShaderHandler()
{
	shaders.clear();
}

bool ShaderHandler::Initialize()
{
	std::shared_ptr<Shader> objShader = std::make_shared<OBJShader>();

	if(objShader->Initialize())
	{
		InsertNewPair("objshader", objShader);
	}

	return true;
}

