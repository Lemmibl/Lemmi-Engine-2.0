#pragma once
#include "../ShaderUniformStructs.h"
#include "../Objects/MaterialStruct.h"
#include "shader.h"

#include <glm/glm.hpp>

class OBJShader : public Shader
{
public:
	OBJShader();
	~OBJShader();

	virtual	bool Initialize();
	void SetupBuffers(GLuint matrixUBO, GLuint materialUBO);
	void BindVPMatrixBuffer(GLuint matrixUBO);
	void BindModelMatrix(glm::mat4& matrix);
	void BindMaterialBuffer(GLuint materialUBO);

	void BindTextureSlot(GLuint textureID);

	virtual void ResetState();

private:
	GLuint sampler;

	GLuint modelMatrixShaderIndex;
	GLuint viewprojMatrixShaderIndex;
	GLuint viewprojMatrixBindingSlot;

	GLuint materialStructShaderIndex;
	GLuint materialBindingSlot;

	GLint textureShaderIndex;

	//Currently not used, but it might be used in the future if we implement several textures at the same time. Normaltextures etc.
	GLuint diffuseTextureBindingSlot;
};

