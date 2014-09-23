#pragma once
#include <string>
#include <vector>

#include <glew.h>
#include <GLFW/glfw3.h>

#include "../Objects/ShaderTypes.h"

class ShaderHandler
{
private:
	enum ShaderCompileFlags
	{
		COMPILE_VERTEX_SHADER	= 1,
		COMPILE_GEOMETRY_SHADER = 2,
		COMPILE_FRAGMENT_SHADER = 4
	};

public:
	ShaderHandler();
	~ShaderHandler();

	bool Initialize();
	
	GLuint GetShader(ShaderEnums::Name type)
	{
		return shaders[type];
	}

private:
	//filename should explicitly only be the file name, not the path to the file, that's already handled.
	//the type enum is the enum that will be used when outside classes want to fetch it from our internal storage vector
	//compileFlags are used to flag what sort of shader code the shader consists of. (vertex / geometry / fragment)
	bool LoadShader(std::string fileName, ShaderEnums::Name type, unsigned char compileFlags);

	GLuint CreateShader(std::string shader, GLenum type_shader);

private:
	std::vector<GLuint> shaders;
	
	GLenum shaderTypeEnums[3];
	std::string shaderTypeDefines[3];
	std::string baseFilePath;
};

