#pragma once

#include <easylogging++.h>

//Include glew before glfw: http://www.glfw.org/faq.html#2_15
#include <windows.h>
#include <glew.h>
#include <GLFW/glfw3.h>

/************************************************************************/
/* This class is the very core of the engine. Every major system will   
 * be owned by this class. Probably. We'll see.							*/
/************************************************************************/
#include "../GUI/CEGUIWrapper.h"
#include "../Screens/ScreenManager.h"
#include "InputManager.h"


class EngineCore
{
public:
	EngineCore();
	~EngineCore();

	//Will contain/start mainloop
	bool Run();

	bool InitializeSystems();
	void Shutdown();

private:
	bool Update();
	void Render();

	void InitializeErrorLogger();
	bool InitializeGUI();
	bool LoadSettings();


private:
	CEGUIWrapper ceguiWrapper;
	ScreenManager screenManager;
	InputManager* inputManager;
	GLFWwindow* glfwWindow;
	float windowWidth, windowHeight;
	double deltaTime, glfwTime, previousGlfwTime;

	//TODO: Sound?
};

