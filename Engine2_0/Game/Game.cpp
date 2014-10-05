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
	if(!modelHandler.Initialize(&materialHandler, &meshHandler, &textureHandler))
	{
		LOG(ERROR) << ("Couldn't initialize modelHandler in Scene.cpp!");
	}

	currentScene.Load(&modelHandler);

	if(!gameRenderer.Initialize(&materialHandler, &meshHandler, &textureHandler))
	{
		return false;
	}

	return true;
}

bool Game::Update( double deltaTime )
{
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