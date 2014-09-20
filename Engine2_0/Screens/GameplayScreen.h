#pragma once
#include "ScreenBaseClass.h"

#include "CEGUI/CEGUI.h"

//I don't really need glew in this class, but glew bitches if I include glfw before glew
#include <glew.h>
#include <GLFW/glfw3.h>

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

