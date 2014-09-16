#include "Game.h"

Game::Game()
{
}


Game::~Game()
{
}

bool Game::Initialize()
{
	if(!gameRenderer.Initialize())
	{
		return false;
	}

	currentScene.Load();

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
