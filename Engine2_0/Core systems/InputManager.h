#pragma once
#include "CEGUI/CEGUI.h"
#include "GLFW/glfw3.h"

//Currently, this is just a hackjob wrapper that distributes glfw events further down to other systems that use the input.
class InputManager
{
public:
	//Don't forget to call initialize at some point!
	static InputManager& GetInstance()
	{
		static InputManager handler;

		return handler;
	}

	~InputManager();

	void Initialize(GLFWwindow* window);
	void Update(double deltaTime);

	//Setters
	void SetMouseButtonState(int button, bool state);
	void SetMouseScrollState(double xDelta, double yDelta);
	void SetMousePosition(double xDelta, double yDelta);
	void SetKeyState(int keycode, bool state);
	void InjectChar(unsigned int keycode);
	void LockMouseCursor(bool val);

	//Static callback functions
	static void GLFWMouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
	static void GLFWMouseCursorPositionCallback(GLFWwindow* window, double xDelta, double yDelta);
	static void GLFWMouseScrollCallback(GLFWwindow* window, double xDelta, double yDelta);
	static void GLFWErrorCallback(int error, const char* description);
	static void GLFWKeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
	static void GLFWCharCallback(GLFWwindow* window, unsigned int codePoint);
	static void GLFWWindowResizeCallback(GLFWwindow* window, int width, int height);

private:
	//Constructor is private because this is a singleton
	InputManager();
	
	//Conversion functions
	unsigned int GLFWToCEGUIKey(int glfwKey);
	CEGUI::MouseButton GLFWToCEGUIButton(int glfwButton);

private:
	//Ptr to window we're supposed to get updates from
	GLFWwindow* glfwWindow;

	double timeSinceLastInput;

	//This is how often we'll read for input
	const double inputCooldown;
	bool cursorLocked;

	//X and Y deltas
	std::pair<double, double> centerMousePos, mouseDelta;
	std::pair<double, double> mouseWheelDelta;

	double mouseSensitivity;

};

