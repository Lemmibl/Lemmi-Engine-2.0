#include "GameplayScreen.h"

#include "../Game/Game.h"

GameplayScreen::GameplayScreen()
	: ScreenBaseClass(),
	currentGame(nullptr)
{
}


GameplayScreen::~GameplayScreen()
{
	ShutdownGame();
}


bool GameplayScreen::Enter()
{
	//if(!HasBeenInitialized())
	//{
	//if(!Initialize())
	//{
	//	return false;
	//}
	//else
	//{
	//	SetInitialized(true);
	//}
	//}

	if(!Initialize())
	{
		return false;
	}

	SetActivated(true);

	return true;
}


void GameplayScreen::Exit()
{
	SetActivated(false);

	ShutdownGame();

	//Show mouse cursor
	CEGUI::System::getSingleton().getDefaultGUIContext().getMouseCursor().show();
}


bool GameplayScreen::Initialize()
{
	glfwWindow = glfwGetCurrentContext();

	if(!NewGame())
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

	if(!currentGame->Update(deltaTime))
	{
		return false;
	}

	return true;
}


bool GameplayScreen::Render( double deltaTime )
{
	if(!currentGame->Render(deltaTime))
	{
		return false;
	}

	return true;
}


bool GameplayScreen::NewGame()
{
	currentGame = new Game();
	if(currentGame == nullptr)
	{
		return false;
	}

	if(!currentGame->Initialize())
	{
		return false;
	}

	return true;
}


void GameplayScreen::ShutdownGame()
{
	if(currentGame != nullptr)
	{
		currentGame->Shutdown();

		delete currentGame;
		currentGame = nullptr;
	}
}
