#pragma once
#include "../Rendering/GameRenderer.h"
#include "TerrainManager.h"
#include "Scene.h"


class Game
{
public:
	Game();
	~Game();

	bool Initialize();

	bool Update(double deltaTime);
	bool Render(double deltaTime);

private:
	Scene currentScene;
	TerrainManager terrainManager;
	GameRenderer gameRenderer;
};

