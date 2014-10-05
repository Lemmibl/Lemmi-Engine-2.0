#include "InputManager.h"

InputManager::InputManager()	
	:	mouseSensitivity(1.0),
	glfwWindow(nullptr),
	inputCooldown(0.2),
	centerMousePos(std::make_pair<double, double>(500.0, 500.0)),
	mouseWheelDelta(std::make_pair<double, double>(0.0, 0.0))
{
	timeSinceLastInput = 0.0;
	cursorLocked = true;
}


InputManager::~InputManager()
{
	glfwWindow = nullptr;
}

void InputManager::Initialize(GLFWwindow* window)
{
	glfwWindow = window;

	//Make sure we adapt to whatever our default state is
	LockMouseCursor(cursorLocked);
}

void InputManager::Update(double deltaTime)
{
	timeSinceLastInput += deltaTime;

	//Toggle if RMB was pressed
	if(timeSinceLastInput >= inputCooldown && glfwGetMouseButton(glfwWindow, GLFW_MOUSE_BUTTON_2) == GLFW_PRESS)
	{
		//Reset
		timeSinceLastInput = 0.0;

		//Toggle
		cursorLocked = !cursorLocked;

		//Handle
		LockMouseCursor(cursorLocked);
	}
}

/*	Setters.	*/
void InputManager::SetMouseButtonState(int button, bool state )
{
	if(button >= 0 && button <= 8)
	{
		unsigned int buttonCode = GLFWToCEGUIButton(button);

		if(state == true)
		{
			CEGUI::System::getSingleton().getDefaultGUIContext().injectMouseButtonDown(static_cast<CEGUI::MouseButton>(buttonCode));
		}
		else
		{
			CEGUI::System::getSingleton().getDefaultGUIContext().injectMouseButtonUp(static_cast<CEGUI::MouseButton>(buttonCode));
		}


		/*mouseButtonStates[button] = state;*/
	}
}

void InputManager::SetMouseScrollState( double xDelta, double yDelta )
{
	if(yDelta > 0.0)
	{
		CEGUI::System::getSingleton().getDefaultGUIContext().injectMouseWheelChange(static_cast<float>(yDelta));
	}

	mouseWheelDelta.first += xDelta;
	mouseWheelDelta.second += yDelta;
}

void InputManager::SetMousePosition(double xDelta, double yDelta)
{
	if(cursorLocked)
	{	
		//Calculate mouse change since last call
		mouseDelta.first = (xDelta - centerMousePos.first) * mouseSensitivity;
		mouseDelta.second = (yDelta - centerMousePos.second) * mouseSensitivity;

		//Inject change
		CEGUI::System::getSingleton().getDefaultGUIContext().injectMouseMove(static_cast<float>(mouseDelta.first), static_cast<float>(mouseDelta.second));

		//Reset cursor pos after we've received input
		glfwSetCursorPos(glfwWindow, centerMousePos.first, centerMousePos.second);
	}
	else
	{
		//If mouse is unlocked, just insert that position instead
		CEGUI::System::getSingleton().getDefaultGUIContext().injectMousePosition(static_cast<float>(xDelta), static_cast<float>(yDelta));
	}
}

void InputManager::SetKeyState(int keycode, bool state)
{
	if(keycode >= 0 && keycode < 512) // OutputDebugString(
	{
		unsigned int key = GLFWToCEGUIKey(keycode);

		if(state == true && key != 0)
		{
			CEGUI::System::getSingleton().getDefaultGUIContext().injectKeyDown(static_cast<CEGUI::Key::Scan>(key));
		}
		else
		{
			CEGUI::System::getSingleton().getDefaultGUIContext().injectKeyUp(static_cast<CEGUI::Key::Scan>(key));
		}

		//Set glfww code
		//keyStates[keycode] = state;
	}
}

void InputManager::InjectChar(unsigned int keycode)
{
	CEGUI::System::getSingleton().getDefaultGUIContext().injectChar(static_cast<CEGUI::utf32>(keycode));

	//keyEvents.push_back(keycode);
}

//¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤
//¤¤¤¤¤¤¤¤¤																															¤¤¤¤¤¤¤¤¤
//¤¤¤¤¤¤¤¤¤		The below functions are static callback functions that are set to interact with the GLFW event callback system.		¤¤¤¤¤¤¤¤¤
//¤¤¤¤¤¤¤¤¤		GLFW is written in C, and this is the only way to handle C callbacks (by calling static public functions)			¤¤¤¤¤¤¤¤¤
//¤¤¤¤¤¤¤¤¤		For more information: http://www.glfw.org/docs/latest/group__input.html#gac96fd3b9fc66c6f0eebaf6532595338f			¤¤¤¤¤¤¤¤¤
//¤¤¤¤¤¤¤¤¤																															¤¤¤¤¤¤¤¤¤
//¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤

/*
This is the function signature for mouse button callback functions.

Parameters
[in]	window	The window that received the event.
[in]	button	The mouse button that was pressed or released.
[in]	action	One of GLFW_PRESS or GLFW_RELEASE.
[in]	mods	Bit field describing which modifier keys were held down.

*/
void InputManager::GLFWMouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
{
	/*
	#define 	GLFW_RELEASE   0
	The key or button was released. More...

	#define 	GLFW_PRESS   1
	The key or button was pressed. More...

	#define 	GLFW_REPEAT   2
	The key was held down until it repeated.
	*/

	//As we can see in the defines above, if action == 0, it means that the key was released. Hence we'll inject a false, because false means the button isn't down.
	//I don't care about GLFW_REPEAT(2), so I just treat it as a GLFW_PRESS(1).

	//bool buttonAction = (action == 0) ? false : true;

	InputManager::GetInstance().SetMouseButtonState(button, ((action == 0) ? false : true));
}

/* 
This is the function signature for cursor position callback functions.

Parameters
[in]	window	The window that received the event.
[in]	xDelta	The new x-coordinate, in screen coordinates, of the cursor.
[in]	yDelta	The new y-coordinate, in screen coordinates, of the cursor.

*/
void InputManager::GLFWMouseCursorPositionCallback( GLFWwindow* window, double xDelta, double yDelta )
{
	InputManager::GetInstance().SetMousePosition(xDelta, yDelta);
}

/*
This is the function signature for scroll callback functions.

Parameters
[in]	window	The window that received the event.
[in]	xDelta	The scroll offset along the x-axis.
[in]	yDelta	The scroll offset along the y-axis.

*/
void InputManager::GLFWMouseScrollCallback( GLFWwindow* window, double xDelta, double yDelta )
{
	InputManager::GetInstance().SetMouseScrollState(xDelta, yDelta);
}

/*
This is the function signature for error callback functions.

Parameters
[in] error	Error code.
[in] description Text string to describe the error.
[
*/
void InputManager::GLFWErrorCallback( int error, const char* description )
{
	//C function because this is a C callback function
	fputs(description, stderr);
}


/*
This is the function signature for keyboard key callback functions.

Parameters
[in]	window	The window that received the event.
[in]	key	The keyboard key that was pressed or released.
[in]	scancode	The system-specific scancode of the key.
[in]	action	GLFW_PRESS, GLFW_RELEASE or GLFW_REPEAT.
[in]	mods	Bit field describing which modifier keys were held down.

*/
void InputManager::GLFWKeyCallback( GLFWwindow* window, int key, int scancode, int action, int mods)
{
	//As we can see in the defines above, if action == 0, it means that the key was released. Hence we'll inject a false, because false means the button isn't down.
	//I don't care about GLFW_REPEAT(2), so I just treat it as a GLFW_PRESS(1).

	InputManager::GetInstance().SetKeyState(key, ((action == 0) ? false : true));
}

/*
This is the function signature for Unicode character callback functions.

Parameters
[in]	window	The window that received the event.
[in]	codepoint	The Unicode code point of the character.

*/
void InputManager::GLFWCharCallback( GLFWwindow* window, unsigned int codePoint )
{
	InputManager::GetInstance().InjectChar(codePoint);
}


/*
This is the function signature for window resizing callback functions.

Parameters
[in]	window	The window that received the event.
[in]	width	The new width of the window.
[in]	height	The new height of the window.
*/
void InputManager::GLFWWindowResizeCallback( GLFWwindow* window, int width, int height )
{
	CEGUI::System::getSingleton().notifyDisplaySizeChanged(CEGUI::Sizef(static_cast<float>(width), static_cast<float>(height)));
}

unsigned int InputManager::GLFWToCEGUIKey( int glfwKey )
{
	switch(glfwKey)
	{
	case GLFW_KEY_UNKNOWN	: return 0;
	case GLFW_KEY_ESCAPE	: return CEGUI::Key::Escape;
	case GLFW_KEY_F1	: return CEGUI::Key::F1;
	case GLFW_KEY_F2	: return CEGUI::Key::F2;
	case GLFW_KEY_F3	: return CEGUI::Key::F3;
	case GLFW_KEY_F4	: return CEGUI::Key::F4;
	case GLFW_KEY_F5	: return CEGUI::Key::F5;
	case GLFW_KEY_F6	: return CEGUI::Key::F6;
	case GLFW_KEY_F7	: return CEGUI::Key::F7;
	case GLFW_KEY_F8	: return CEGUI::Key::F8;
	case GLFW_KEY_F9	: return CEGUI::Key::F9;
	case GLFW_KEY_F10       : return CEGUI::Key::F10;
	case GLFW_KEY_F11       : return CEGUI::Key::F11;
	case GLFW_KEY_F12       : return CEGUI::Key::F12;
	case GLFW_KEY_F13       : return CEGUI::Key::F13;
	case GLFW_KEY_F14       : return CEGUI::Key::F14;
	case GLFW_KEY_F15       : return CEGUI::Key::F15;
	case GLFW_KEY_UP        : return CEGUI::Key::ArrowUp;
	case GLFW_KEY_DOWN      : return CEGUI::Key::ArrowDown;
	case GLFW_KEY_LEFT      : return CEGUI::Key::ArrowLeft;
	case GLFW_KEY_RIGHT     : return CEGUI::Key::ArrowRight;
	case GLFW_KEY_LEFT_SHIFT    : return CEGUI::Key::LeftShift;
	case GLFW_KEY_RIGHT_SHIFT    : return CEGUI::Key::RightShift;
	case GLFW_KEY_LEFT_CONTROL     : return CEGUI::Key::LeftControl;
	case GLFW_KEY_RIGHT_CONTROL    : return CEGUI::Key::RightControl;
	case GLFW_KEY_LEFT_ALT      : return CEGUI::Key::LeftAlt;
	case GLFW_KEY_RIGHT_ALT      : return CEGUI::Key::RightAlt;
	case GLFW_KEY_TAB       : return CEGUI::Key::Tab;
	case GLFW_KEY_ENTER     : return CEGUI::Key::Return;
	case GLFW_KEY_BACKSPACE : return CEGUI::Key::Backspace;
	case GLFW_KEY_INSERT    : return CEGUI::Key::Insert;
	case GLFW_KEY_DELETE       : return CEGUI::Key::Delete;
	case GLFW_KEY_PAGE_UP    : return CEGUI::Key::PageUp;
	case GLFW_KEY_PAGE_DOWN  : return CEGUI::Key::PageDown;
	case GLFW_KEY_HOME      : return CEGUI::Key::Home;
	case GLFW_KEY_END       : return CEGUI::Key::End;
	case GLFW_KEY_KP_ENTER	: return CEGUI::Key::NumpadEnter;
	default			: return 0;
	}
}

CEGUI::MouseButton InputManager::GLFWToCEGUIButton( int glfwButton )
{
	switch(glfwButton)
	{
	case GLFW_MOUSE_BUTTON_LEFT	: return CEGUI::LeftButton;
	case GLFW_MOUSE_BUTTON_RIGHT	: return CEGUI::RightButton;
	case GLFW_MOUSE_BUTTON_MIDDLE	: return CEGUI::MiddleButton;
	default				: return CEGUI::NoButton;
	}
}

void InputManager::LockMouseCursor(bool val)
{
	cursorLocked = val;

	if(cursorLocked)
	{
		glfwSetInputMode(glfwWindow, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
	}
	else
	{
		glfwSetInputMode(glfwWindow, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
	}

	glfwSetCursorPos(glfwWindow, centerMousePos.first, centerMousePos.second);
}
