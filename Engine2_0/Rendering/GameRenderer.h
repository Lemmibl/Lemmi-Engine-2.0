#pragma once
#include <glew.h>
#include <GLFW/glfw3.h>

#include <vector>
#include <memory>
#include <functional>

#include "Object handlers/ShaderHandler.h"
#include "../Game/Scene.h"

#include "ShaderUniformStructs.h"
#include "Shader code/OBJShader.h"

// glm::vec3, glm::vec4, glm::ivec4, glm::mat4
#include <glm/glm.hpp>

// glm::translate, glm::rotate, glm::scale, glm::perspective
#include <glm/gtc/matrix_transform.hpp>

// glm::value_ptr
#include <glm/gtc/type_ptr.hpp> 

class Scene;
class MaterialHandler;
class MeshHandler;
class TextureHandler;

class GameRenderer
{
public:
	GameRenderer();
	~GameRenderer();

	bool Initialize(MaterialHandler* mtlHandlerPtr, MeshHandler* meshHandlerPtr, TextureHandler* texHandlerPtr, TransformHandler* transformHandlerPtr);

	void Update(double deltaTime);
	void Render(Scene* scene);

private:
	void SetupRenderPasses();


private:
	GLFWwindow* glfwWindow;
	int screenWidth, screenHeight;
	float ratio;
	double rotation;

	std::vector<std::function<void(Scene*)>> renderPasses;
	ShaderHandler shaderManager;

	MaterialHandler* materialHandler;
	MeshHandler* meshHandler; 
	TextureHandler* textureHandler;
	TransformHandler* transformHandler;

	Matrices wvpMatrixStruct;
	GLuint matrixStructShaderIndex, matrixBindingIndex, matrixUBO;
	OBJShader* objShader;

	glm::mat4x4 scaleMatrix;
};
