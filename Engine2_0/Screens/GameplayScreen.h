#pragma once
#include "ScreenBaseClass.h"

#include "../Game/Game.h"
#include "CEGUI/CEGUI.h"
#include <GLFW/glfw3.h>

class GameplayScreen : public ScreenBaseClass
{
public:
	GameplayScreen();
	~GameplayScreen();

	virtual bool Enter();
	virtual void Exit();

	bool Initialize();

	virtual bool Update(double deltaTime);
	virtual bool Render(double deltaTime);

private:
	GLFWwindow* glfwWindow;
	Game currentGame;
};

