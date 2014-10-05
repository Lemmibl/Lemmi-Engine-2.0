#include "Shader.h"

#include <iostream>
#include <fstream>
#include <string>
#include <vector>

static const GLenum shaderTypeEnums[3] = { GL_VERTEX_SHADER, GL_GEOMETRY_SHADER, GL_FRAGMENT_SHADER };
static const std::string shaderTypeDefines[3] = { "#define VERTEX_SHADER\n", "#define GEOMETRY_SHADER\n", "#define FRAGMENT_SHADER\n" };
static const std::string baseFilePath = "../Engine2_0/Data/Shaders/";

Shader::Shader()						  
{
}

Shader::~Shader()
{
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
		if(shaderCode.length() == 0)
		{
			return false;
		}

		// Link the program
		programID = glCreateProgram();

		if(programID == 0)
		{
			return false;
		}

		unsigned char index = 0;
		GLuint shaderIDs[3];

		//Alright, so we're doing 1 -> 2 -> 4 to bitwise test against the flags.
		//This way I don't need to make different functions and a bunch of really specific if checks to see if I have a geometry shader or not etc
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

		//Vars to hold results
		GLint result = GL_FALSE;
		int infoLogLength;

		// Check the program
		glGetProgramiv(programID, GL_LINK_STATUS, &result);

		if(result == GL_FALSE)
		{

			glGetProgramiv(programID, GL_INFO_LOG_LENGTH, &infoLogLength);
			std::vector<char> programErrorMessage(std::max(infoLogLength, int(1)) );
			glGetProgramInfoLog(programID, infoLogLength, NULL, &programErrorMessage[0]);
			fprintf(stdout, "%s\n", &programErrorMessage[0]);
		}

		index = 0;

		//Loop through again and delete any shaders
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

		glValidateProgram(programID);
		glGetProgramiv(programID, GL_VALIDATE_STATUS, &result);
		if(result == GL_FALSE)
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

	if(shader_id != 0)
	{
		const char* glsl_cstr(shader.c_str());

		glShaderSource(shader_id, 1, &glsl_cstr, NULL);
		glCompileShader(shader_id);

		//Vars to hold feedback
		GLint result = GL_FALSE;

		//Fetch information
		glGetShaderiv(shader_id, GL_COMPILE_STATUS, &result);

		//If compilation went wrong
		if(result == GL_FALSE)
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
