#pragma once
#include "../ShaderUniformStructs.h"
#include "../Objects/MaterialStruct.h"
#include "shader.h"

class OBJShader : public Shader
{
public:
	OBJShader();
	~OBJShader();

	virtual	bool Initialize();
	void SetupBuffers(GLuint matrixUBO, GLuint materialUBO);
	void BindBuffers(GLuint matrixUBO, GLuint materialUBO);

	void BindTextureSlot(GLuint textureID);

	virtual void ResetState();

private:
	GLuint sampler;

	GLuint matrixStructShaderIndex;
	GLuint matrixBindingSlot;

	GLuint materialStructShaderIndex;
	GLuint materialBindingSlot;

	GLint textureShaderIndex;
	GLuint diffuseTextureBindingSlot;
};

