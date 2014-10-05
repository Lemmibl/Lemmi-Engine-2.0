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
	void UpdateMatrixBuffer(Matrices& matrixStruct, GLuint matrixUBO);
	void UpdateMaterialBuffer(MaterialStruct& materialStruct, GLuint materialUBO);

	void BindBuffers(GLuint matrixUBO, GLuint materialUBO);

private:
	GLuint matrixStructShaderIndex;
	GLuint matrixBindingSlot;

	GLuint materialStructShaderIndex;
	GLuint materialBindingSlot;
};

