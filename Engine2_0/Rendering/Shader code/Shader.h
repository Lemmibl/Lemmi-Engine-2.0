#pragma once
#include <glew.h>
#include <GLFW/glfw3.h>
#include <string>

//Base class for all shaders
class Shader
{
public:
	Shader();
	virtual ~Shader();

	void Enable() { glUseProgram(programID); }
	void Disable() 
	{ 
		ResetState();
		glUseProgram(0); 
	}

	virtual bool Initialize() = 0;

protected:
	//they're "binary" because I want to be able to combine them into a single flag variable, and as such they have to be able to fill their own bit each
	enum ShaderCompileFlags
	{
		COMPILE_VERTEX_SHADER	= 1,
		COMPILE_GEOMETRY_SHADER = 2,
		COMPILE_FRAGMENT_SHADER = 4
	};

	//Function that each shader can overload if it wants to.
	virtual void ResetState();

	bool LoadShader(std::string fileName, unsigned char compileFlags);
	std::string GetBaseFilepath();

private:
	GLuint CreateShader(std::string shader, GLenum type_shader);

protected:
	//Index to where the actual program is stored inside ogl
	GLuint programID;
};