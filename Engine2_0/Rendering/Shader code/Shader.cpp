#include "Shader.h"

#include <iostream>
#include <fstream>
#include <string>
#include <vector>

//Observe that these are opengl enums, and as such aren't 0, 1, 2 etc by value
static const GLenum shaderTypeEnums[3] = { GL_VERTEX_SHADER, GL_GEOMETRY_SHADER, GL_FRAGMENT_SHADER };

//Strings that we match against to find the right section of code that we want to use
static const std::string shaderTypeDefines[3] = { "#define VERTEX_SHADER\n", "#define GEOMETRY_SHADER\n", "#define FRAGMENT_SHADER\n" };

//Where all the shader files should be located
static const std::string baseFilePath = "../Engine2_0/Data/Shaders/";

Shader::Shader()						  
{
}

Shader::~Shader()
{
}

void Shader::ResetState()
{
	//By default empty, but it's virtual so can be overloaded by any derived class if functionality is needed
}

//http://www.opengl-tutorial.org/beginners-tutorials/tutorial-2-the-first-triangle/
bool Shader::LoadShader(std::string fileName, unsigned char compileFlags)
{
	//So we create a filestrean with the complete filepath
	std::ifstream shaderStream(fileName, std::ios::in);

	if(shaderStream.is_open())
	{
		//Read shader code from file through stream
		std::string shaderCode((std::istreambuf_iterator<char>(shaderStream)), std::istreambuf_iterator<char>());

		shaderStream.close();

		//Break if something went wrong...
		if(0 == shaderCode.length())
		{
			return false;
		}

		// Link the program
		programID = glCreateProgram();

		if(0 == programID)
		{
			return false;
		}

		unsigned char index = 0;
		GLuint shaderIDs[3];

		//Alright, so we're doing 1 -> 2 -> 4 to bitwise test against the flags.
		//This way I don't need to make different functions and a bunch of really specific if checks to see if I have a geometry shader or not etc
		//binary increase: 1 -> 2 -> 4
		for(unsigned char i = 1; i <= 4; i *= 2)
		{
			//So comparing against compile flags means that first loop through == Vertex,
			//second loop through == Geometry and third is == Fragment
			if(i & compileFlags)
			{
				//We save the IDs for later when we need to delete them
				shaderIDs[index] = CreateShader(shaderTypeDefines[index]+shaderCode, shaderTypeEnums[index]);

				//Check if results are OK
				if(shaderIDs[index] != 0)
				{
					//And append
					glAttachShader(programID, shaderIDs[index]);
				}
			}

			//We need a normal index that goes 0, 1, 2, 3 etc because the i var is going to be stepping in binary, á la 1, 2, 4, 8
			++index;
		}

		//Finally link everything
		glLinkProgram(programID);

		//Vars to hold potential errors
		GLint result = GL_FALSE;
		int infoLogLength;

		// Check the program for problems
		glGetProgramiv(programID, GL_LINK_STATUS, &result);

		//Output error messages if something went wrong
		if(GL_FALSE == result)
		{
			glGetProgramiv(programID, GL_INFO_LOG_LENGTH, &infoLogLength);
			std::vector<char> programErrorMessage(std::max(infoLogLength, int(1)) );
			glGetProgramInfoLog(programID, infoLogLength, NULL, &programErrorMessage[0]);
			fprintf(stdout, "%s\n", &programErrorMessage[0]);
		}

		index = 0;

		//Loop through again and delete any shaders
		//binary increase: 1 -> 2 -> 4
		for(unsigned char i = 1; i < 4; i *= 2)
		{
			//So comparing against compile flags means that first loop through == Vertex,
			//second loop through == Geometry and third is == Fragment
			if(i & compileFlags)
			{
				glDeleteShader(shaderIDs[index]);
			}

			++index;
		}

		//Output error messages again to see if something went wrong when validating a final time
		glValidateProgram(programID);
		glGetProgramiv(programID, GL_VALIDATE_STATUS, &result);
		if(GL_FALSE == result)
		{
			return false;
		}
	}
	else
	{
		//Error.
		return false;
	}

	return true;
}

GLuint Shader::CreateShader(std::string shader, GLenum type_shader)
{
	GLuint shader_id = glCreateShader(type_shader);

	if(0 != shader_id)
	{
		const char* glsl_cstr(shader.c_str());

		glShaderSource(shader_id, 1, &glsl_cstr, NULL);
		glCompileShader(shader_id);

		//Vars to hold feedback
		GLint result = GL_FALSE;

		//Fetch information
		glGetShaderiv(shader_id, GL_COMPILE_STATUS, &result);

		//If compilation went wrong
		if(GL_FALSE == result)
		{	
			int infoLogLength;

			glGetShaderiv(shader_id, GL_INFO_LOG_LENGTH, &infoLogLength);

			std::vector<char> shaderErrorMessage(infoLogLength);
			glGetShaderInfoLog(shader_id, infoLogLength, NULL, &shaderErrorMessage[0]);
			fprintf(stdout, "%s\n", &shaderErrorMessage[0]);
		}
	}

	return shader_id;
}

std::string Shader::GetBaseFilepath()
{
	return baseFilePath;
}