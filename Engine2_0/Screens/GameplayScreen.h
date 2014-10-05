#pragma once
#include "../Game/Game.h"
#include "CEGUI/CEGUI.h"
#include <glew.h>
#include <GLFW/glfw3.h>

#include "ScreenBaseClass.h"

class Game;

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
	bool NewGame();
	void ShutdownGame();

private:
	GLFWwindow* glfwWindow;
	Game* currentGame;
};

