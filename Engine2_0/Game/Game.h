#pragma once
#include "../Rendering/GameRenderer.h"
#include "../Terrain/TerrainManager.h"
#include "Scene.h"

#include "../Rendering/Object handlers/MaterialHandler.h"
#include "../Rendering/Object handlers/MeshHandler.h"
#include "../Rendering/Object handlers/TextureHandler.h"
#include "../Rendering/Object handlers/ModelHandler.h"

class Game
{
public:
	Game();
	~Game();
	bool Shutdown();

	bool Initialize();

	bool Update(double deltaTime);
	bool Render(double deltaTime);

private:
	Scene currentScene;
	GameRenderer gameRenderer;

	TerrainManager terrainManager;

	//Models are aggregate objects consisting of mesh handles, material handles and texture handles
	ModelHandler modelHandler;

	MaterialHandler materialHandler;
	MeshHandler meshHandler;
	TextureHandler textureHandler;
};

