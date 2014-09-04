#pragma once

/************************************************************************/
/* This class is the very core of the engine. Every major system will   
 * be owned by this class. Probably. We'll see.							*/
/************************************************************************/
#include "../GUI/CEGUIWrapper.h"
#include "../Screens/ScreenManager.h"
#include "InputManager.h"

//Include glfw last! I can't remember why but the manual tells you to do so.
#include <GLFW/glfw3.h>

class EngineCore
{
public:
	EngineCore();
	~EngineCore();

	//Will contain/start mainloop
	bool Run();

	bool InitializeSystems();
	
private:
	void Shutdown();
	bool Update();
	void Render();

	bool InitializeGUI();
	bool LoadSettings();


private:
	CEGUIWrapper ceguiWrapper;
	ScreenManager screenManager;
	InputManager* inputManager;
	GLFWwindow* glfwWindow;
	float windowWidth, windowHeight;
	double deltaTime, glfwTime;

	//TODO: Sound?
};

