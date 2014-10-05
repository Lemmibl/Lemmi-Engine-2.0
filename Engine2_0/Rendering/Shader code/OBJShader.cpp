#include "OBJShader.h"

// glm::vec3, glm::vec4, glm::ivec4, glm::mat4
#include <glm/glm.hpp>

// glm::value_ptr
#include <glm/gtc/type_ptr.hpp> 
#include "../ShaderUniformStructs.h"

OBJShader::OBJShader() : Shader()
{
	matrixBindingSlot = 0;
	materialBindingSlot = 1;
}


OBJShader::~OBJShader()
{
}

bool OBJShader::Initialize()
{
	if(LoadShader(GetBaseFilepath() + "OBJShader.glsl", (COMPILE_VERTEX_SHADER | COMPILE_FRAGMENT_SHADER)))
	{
		return true;
	}

	return false;
}

void OBJShader::SetupBuffers(GLuint matrixUBO, GLuint materialUBO)
{	
	glUseProgram(programID);

	//Material setup
	materialStructShaderIndex = glGetUniformBlockIndex(programID, "Material");
	glBindBufferRange(GL_UNIFORM_BUFFER, materialStructShaderIndex, materialUBO, 0, sizeof(MaterialStruct));
	glUniformBlockBinding(programID, materialStructShaderIndex, materialBindingSlot);


	//Matrix setup
	matrixStructShaderIndex = glGetUniformBlockIndex(programID, "Matrices");
	glBindBufferRange(GL_UNIFORM_BUFFER, matrixStructShaderIndex, matrixUBO, 0, sizeof(Matrices));
	glUniformBlockBinding(programID, matrixStructShaderIndex, matrixBindingSlot);
}

void OBJShader::UpdateMatrixBuffer(Matrices& matrixStruct, GLuint matrixUBO)
{

}

void OBJShader::UpdateMaterialBuffer( MaterialStruct& materialStruct, GLuint materialUBO )
{

}

void OBJShader::BindBuffers(GLuint matrixUBO, GLuint materialUBO)
{
	glBindBufferRange(GL_UNIFORM_BUFFER, matrixBindingSlot, matrixUBO, 0, sizeof(Matrices));
	//TODO: material
}

