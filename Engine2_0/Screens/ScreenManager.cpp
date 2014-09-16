#include "ScreenManager.h"

#include "ScreenBaseClass.h"
#include "MainMenuScreen.h"
#include "GameplayScreen.h"
#include "OptionsScreen.h"

#include <GLFW/glfw3.h>

ScreenManager::ScreenManager()
: running(true),
currentScreen(nullptr),
glfwTime(0.0)
{
}


ScreenManager::~ScreenManager()
{
	running = false;
	currentScreen = nullptr;
}

bool ScreenManager::Initialize()
{
	//New some screens into unique pointers
	std::unique_ptr<MainMenuScreen> mainMenuScreen(new MainMenuScreen);
	std::unique_ptr<GameplayScreen> gameplayScreen(new GameplayScreen);

	//MOVE pointers into function and associate the screens to the right enums
	AddNewState(ScreenStates::MainMenu, std::move(mainMenuScreen));
	AddNewState(ScreenStates::InGame, std::move(gameplayScreen));

	//Here, the unique pointers are no longer valid! Beware.

	//Set current state to main menu
	SwitchState(ScreenStates::MainMenu);

	return true;
}

bool ScreenManager::Update(double deltaTime)
{
	//Terminate
	if(!running)
	{
		return false;
	}

	//glfwTime = glfwGetTime();

	//I've also made sure to have the option to let the individual states flag that the program should shut down
	if(currentScreen->IsActive())
	{
		if(!currentScreen->Update(deltaTime))
		{
			return false;
		}
	}

	return true;
}

void ScreenManager::Render(double deltaTime)
{
	currentScreen->Render(deltaTime);
}

void ScreenManager::SwitchState(ScreenStates::State newScreenEnum)
{
	//Special case...
	if(newScreenEnum == ScreenStates::Quit)
	{
		currentScreen = nullptr;
		running = false;
		return;
	}

	//Look for the new state in our map
	auto& newScreen = screenLookupTable.find(newScreenEnum);

	//If newState has been found in our map
	if(newScreen != screenLookupTable.cend())
	{
		//Exit old screen.. if there was one.
		if(currentScreen != nullptr)
		{
			currentScreen->Exit();
		}

		//Change current screen to new one
		currentScreen = newScreen->second.get();

		//Aaaand... Enter new screen
		if(!currentScreen->Enter())
		{
			//FAILURE
			currentScreen = nullptr;
			running = false;
			return;
		}
		else
		{
			//SUCCESS
			running = true;
		}
	}
	else
	{
		//Something has gone terribly wrong if we've reached this point. Better shut down.
		//std::cerr << "Tried to change to a state that doesn't exist." << std::endl;

		return;
	}
}

void ScreenManager::AddNewState(ScreenStates::State screenEnum, std::unique_ptr<ScreenBaseClass> screen)
{
	//Technically speaking not a key, but an iterator into the map, but for all intents and purposes, I use it as a key here.
	auto& key = screenLookupTable.find(screenEnum);

	//If this key doesn't already exist in the map
	if(key == screenLookupTable.cend())
	{
		//Append event to out SwitchState function. This is the event that a state will throw if it wants to start transitioning to another state.
		screen->GetStateChangeEvent()->Add(*this, &ScreenManager::SwitchState);

		//MOVE ptrs into map
		screenLookupTable.insert(std::make_pair(screenEnum, std::move(screen)));
	}
}