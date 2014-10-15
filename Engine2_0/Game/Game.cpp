#include "Game.h"

#include <easylogging++.h>

Game::Game()
{
}


Game::~Game()
{
}

bool Game::Shutdown()
{

	return true;
}

bool Game::Initialize()
{
	if(!modelHandler.Initialize(&materialHandler, &meshHandler, &textureHandler, &transformHandler))
	{
		LOG(ERROR) << ("Couldn't initialize modelHandler in Scene.cpp");
		return false;
	}

	currentScene.Load(&modelHandler, &transformHandler);

	if(!gameRenderer.Initialize(&materialHandler, &meshHandler, &textureHandler, &transformHandler))
	{
		LOG(ERROR) << ("Couldn't initialize gameRenderer in Scene.cpp");
		return false;
	}

	return true;
}

bool Game::Update( double deltaTime )
{
	transformHandler.Update(deltaTime);

	//Update last
	gameRenderer.Update(deltaTime);
	
	return true;
}

bool Game::Render( double deltaTime )
{
	//Call last
	gameRenderer.Render(&currentScene);

	return true;
}