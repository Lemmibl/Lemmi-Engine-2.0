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
			SetActivated(true);
		}
	}

	return true;
}

void GameplayScreen::Exit()
{
	SetActivated(false);
}

bool GameplayScreen::Initialize()
{
	glfwWindow = glfwGetCurrentContext();

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
	if(currentGame.Render(deltaTime))
	{
		return false;
	}

	return true;
}
