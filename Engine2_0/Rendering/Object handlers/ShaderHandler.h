#pragma once
#include <glew.h>
#include <GLFW/glfw3.h>

#include <string>
#include <unordered_map>
#include <memory>

#include "../Shader code/Shader.h"

class ShaderHandler
{
public:
	ShaderHandler();
	~ShaderHandler();

	bool Initialize();
	
	bool GetShader(std::string name, Shader** outShader)
	{
		auto it = shaders.find(name);

		if(it != shaders.end())
		{
			*outShader = it->second.get();
			return true;
		}

		return false;
	}

private:
	void InsertNewPair(std::string shaderName, std::shared_ptr<Shader> shaderPtr)
	{
		shaders.insert(std::make_pair<std::string, std::shared_ptr<Shader>>(shaderName, shaderPtr));
	}

private:
	std::unordered_map<std::string, std::shared_ptr<Shader>> shaders;	
};

