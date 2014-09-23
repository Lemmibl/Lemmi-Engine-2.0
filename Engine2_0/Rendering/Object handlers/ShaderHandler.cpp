#include "ShaderHandler.h"

#include <iostream>
#include <fstream>
#include <string>

ShaderHandler::ShaderHandler()
	:	shaders(ShaderEnums::SHADER_COUNT), //Initialize shaders vector to have the right amount of slots
		baseFilePath("../Engine2_0/Data/Shaders/")
{
	shaderTypeDefines[0] = "#define VERTEX_SHADER\n";
	shaderTypeDefines[1] = "#define GEOMETRY_SHADER\n";
	shaderTypeDefines[2] = "#define FRAGMENT_SHADER\n";

	shaderTypeEnums[0] = GL_VERTEX_SHADER;
	shaderTypeEnums[1] = GL_GEOMETRY_SHADER;
	shaderTypeEnums[2] = GL_FRAGMENT_SHADER;
}


ShaderHandler::~ShaderHandler()
{
}

bool ShaderHandler::Initialize()
{
	unsigned char compileFlags = (COMPILE_VERTEX_SHADER | COMPILE_FRAGMENT_SHADER);

	if(!LoadShader("GeneralShader.glsl", ShaderEnums::GeneralShader, compileFlags))
	{
		return false;
	}

	return true;
}

//http://www.opengl-tutorial.org/beginners-tutorials/tutorial-2-the-first-triangle/
bool ShaderHandler::LoadShader(std::string fileName, ShaderEnums::Name shaderName, unsigned char compileFlags)
{
	//So we create a filestrean with the complete filepath
	std::ifstream shaderStream(baseFilePath+fileName, std::ios::in);

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
		GLuint programID = glCreateProgram();

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

		//Finally, save the shader ID
		shaders[shaderName] = programID;
	}
	else
	{
		//Error.
		return false;
	}

	return true;
}

GLuint ShaderHandler::CreateShader(std::string shader, GLenum type_shader)
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