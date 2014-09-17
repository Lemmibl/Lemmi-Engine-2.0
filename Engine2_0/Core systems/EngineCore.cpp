#include "EngineCore.h"


EngineCore::EngineCore()
	//In the future these will be loaded from... say, a config file
	: windowWidth(800.0f), windowHeight(600.0f),
	deltaTime(0.0), previousGlfwTime(0.0),
	ceguiWrapper(), screenManager() //I don't necessarily need to call these constructors, it will happen automatically, but I like to decide in what order to call them.
{
}


EngineCore::~EngineCore()
{
	Shutdown();
}


void EngineCore::Shutdown()
{
	glfwDestroyWindow(glfwWindow);
	glfwTerminate();
	glfwWindow = nullptr;
}


bool EngineCore::InitializeSystems()
{
	//Save a ptr to handler to save a few instructions on all those ::GetInstance() calls that happen in the update loop.
	inputManager = &InputManager::GetInstance(); 

	if(!LoadSettings())
	{
		return false;
	}

	if(!InitializeGUI())
	{
		return false;
	}

	screenManager.Initialize();

	return true;
}


bool EngineCore::InitializeGUI()
{
	//Bind error callback function first of all, so that it gets called if anything goes wrong
	glfwSetErrorCallback(InputManager::GLFWErrorCallback);

	/* Initialize the library */
	if(!glfwInit())
	{
		return false;
	}

	/* Create a windowed mode window and its OpenGL context */
	glfwWindow = glfwCreateWindow(static_cast<int>(windowWidth), static_cast<int>(windowHeight), "Engine 2.0", NULL, NULL);
	if(!glfwWindow)
	{
		glfwTerminate();
		return false;
	}

	/* Make the window's context current */
	glfwMakeContextCurrent(glfwWindow); // Initialize GLEW 
	

	glewExperimental = GL_TRUE;

	//Initialize GLEW
	if (glewInit() != GLEW_OK)
	{
		return false;
	}

	//Bind all the callback functions that GLFW calls upon events
	glfwSetKeyCallback(glfwWindow,					InputManager::GLFWKeyCallback);
	glfwSetCharCallback(glfwWindow,					InputManager::GLFWCharCallback);
	glfwSetMouseButtonCallback(glfwWindow,			InputManager::GLFWMouseButtonCallback);
	glfwSetScrollCallback(glfwWindow,				InputManager::GLFWMouseScrollCallback);
	glfwSetCursorPosCallback(glfwWindow,			InputManager::GLFWMouseCursorPositionCallback);
	glfwSetWindowSizeCallback(glfwWindow,			InputManager::GLFWWindowResizeCallback);

	//Make sure inputhandler has a valid ptr to the current window.
	inputManager->Initialize(glfwWindow);

	//Initialize CEGUI
	if(!ceguiWrapper.Initialize())
	{
		return false;
	}

	return true;
}


bool EngineCore::LoadSettings()
{
	return true;
}


bool EngineCore::Run()
{
	//This is our main loop. We enter it and don't leave until update returns false.
	for(;;)
	{
		if(!Update())
		{
			return false;
		}

		Render();
	}

	return true;
}


bool EngineCore::Update()
{
	/*
	http://www.glfw.org/GLFWReference276.pdf

	This function is used for polling for events, such as user input and window resize events. Upon calling
	this function, all window states, keyboard states and mouse states are updated. If any related callback
	functions are registered, these are called during the call to glfwPollEvents.
	*/
	glfwPollEvents();

	glfwTime = glfwGetTime();
	deltaTime = (glfwTime - previousGlfwTime);

	inputManager->Update(deltaTime);

	//See if we should shut down application
	if(glfwWindowShouldClose(glfwWindow))
	{
		return false;
	}

	//Update all CEGUI elements. Inject delta time.
	ceguiWrapper.Update(deltaTime);

	//Update currently active screen/state	
	if(!screenManager.Update(deltaTime))
	{
		return false;
	}

	//Save for next time
	previousGlfwTime = glfwTime;

	return true;
}


void EngineCore::Render()
{
	//Render all GUI stuff
	ceguiWrapper.Render();
	
	// Render whatever state is currently active. 
	// Not even sure if this is needed because all the states are just different menus, and they should get rendered when ceguiwrapper::render is called.
	// I keep it in for future proofing.
	screenManager.Render(deltaTime);

	//glfw stuff
	glfwSwapBuffers(glfwWindow);
}