#pragma once
#include <vector>
#include <memory>
#include <functional>

#include <glew.h>
#include <GLFW/glfw3.h>
#include "math_3d.h"

#include "Object handlers/ShaderHandler.h"
#include "Objects/ShaderTypes.h"
#include "../Game/Scene.h"

class Scene;

class GameRenderer
{
public:
	GameRenderer();
	~GameRenderer();

	bool Initialize();

	void Update(double deltaTime);
	void Render(Scene* scene);

private:
	void SetupRenderPasses();


private:
	GLFWwindow* glfwWindow;
	int screenWidth, screenHeight;
	float ratio;

	std::vector<std::function<void(Scene*)>> renderPasses;
	ShaderHandler shaderManager;
};
