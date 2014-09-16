#include "GameplayScreen.h"

GameplayScreen::GameplayScreen()
: ScreenBaseClass()
{
}


GameplayScreen::~GameplayScreen()
{
}

bool GameplayScreen::Enter()
{
	if(!HasBeenInitialized())
	{
		if(!Initialize())
		{
			return false;
		}
		else
		{
			SetInitialized(true);
		}
	}

	//TODO: Turn off CEGUI completely here? Look in old Engine

	SetActivated(true);

	return true;
}

void GameplayScreen::Exit()
{
	SetActivated(false);

	//Show mouse cursor
	CEGUI::System::getSingleton().getDefaultGUIContext().getMouseCursor().show();
}

bool GameplayScreen::Initialize()
{
	glfwWindow = glfwGetCurrentContext();

	if(!currentGame.Initialize())
	{
		return false;
	}

	return true;
}

bool GameplayScreen::Update( double deltaTime )
{
	//If escape was pressed, return to the main menu
	if(glfwGetKey(glfwWindow, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		stateChangeEvent(ScreenStates::MainMenu);
	}

	if(!currentGame.Update(deltaTime))
	{
		return false;
	}

	return true;
}

bool GameplayScreen::Render( double deltaTime )
{
	if(!currentGame.Render(deltaTime))
	{
		return false;
	}

	return true;
}
