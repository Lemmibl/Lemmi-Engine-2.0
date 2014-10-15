#include "OBJShader.h"

// glm::vec3, glm::vec4, glm::ivec4, glm::mat4
#include <glm/glm.hpp>

// glm::value_ptr
#include <glm/gtc/type_ptr.hpp> 
#include "../ShaderUniformStructs.h"

OBJShader::OBJShader() : Shader()
{
	viewprojMatrixBindingSlot = 0;
	materialBindingSlot = 1;

	diffuseTextureBindingSlot = 0;
}


OBJShader::~OBJShader()
{
}

bool OBJShader::Initialize()
{
	if(!LoadShader(GetBaseFilepath() + "OBJShader.glsl", (COMPILE_VERTEX_SHADER | COMPILE_FRAGMENT_SHADER)))
	{
		return false;
	}

	glGenSamplers(1, &sampler);
	glBindSampler(0, sampler); 
	glSamplerParameteri(sampler, GL_TEXTURE_WRAP_S, GL_WRAP_BORDER);
	glSamplerParameteri(sampler, GL_TEXTURE_WRAP_T, GL_WRAP_BORDER);
	glSamplerParameteri(sampler, GL_TEXTURE_WRAP_R, GL_WRAP_BORDER);
	glSamplerParameteri(sampler, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glSamplerParameteri(sampler, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glSamplerParameteri(sampler, GL_TEXTURE_MAX_ANISOTROPY_EXT, 16);
	glBindSampler(0, 0); 


	return true;
}

void OBJShader::SetupBuffers(GLuint matrixUBO, GLuint materialUBO)
{	
	glUseProgram(programID);

	//Texture setup
	glActiveTexture(GL_TEXTURE0);
	textureShaderIndex = glGetUniformLocation(programID, "diffuseTex");

	//Material setup
	materialStructShaderIndex = glGetUniformBlockIndex(programID, "Material");
	//glBindBufferRange(GL_UNIFORM_BUFFER, materialStructShaderIndex, materialUBO, 0, sizeof(MaterialStruct));
	glUniformBlockBinding(programID, materialStructShaderIndex, materialBindingSlot);


	//Matrix setup
	modelMatrixShaderIndex = glGetUniformLocation(programID, "modelMatrix");
	viewprojMatrixShaderIndex = glGetUniformBlockIndex(programID, "Matrices");
	//glBindBufferRange(GL_UNIFORM_BUFFER, matrixStructShaderIndex, matrixUBO, 0, sizeof(Matrices));
	glUniformBlockBinding(programID, viewprojMatrixShaderIndex, viewprojMatrixBindingSlot);
}

void OBJShader::BindVPMatrixBuffer(GLuint matrixUBO)
{
	glBindBufferRange(GL_UNIFORM_BUFFER, viewprojMatrixBindingSlot, matrixUBO, 0, sizeof(Matrices));
}

void OBJShader::BindModelMatrix(glm::mat4& matrix)
{
	glUniformMatrix4fv(modelMatrixShaderIndex, 1, GL_FALSE, glm::value_ptr(matrix));
}

void OBJShader::BindMaterialBuffer( GLuint materialUBO )
{

}

void OBJShader::BindTextureSlot(GLuint textureID)
{
	glBindSampler(0, sampler); 

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, textureID);
	glUniform1i(textureShaderIndex, 0);
}

void OBJShader::ResetState()
{
	glBindTexture(GL_TEXTURE_2D, 0);
	glBindSampler(0, 0); 
}