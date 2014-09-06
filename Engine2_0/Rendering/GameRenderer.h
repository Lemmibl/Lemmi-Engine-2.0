#pragma once
class Scene;

class GameRenderer
{
public:
	GameRenderer();
	~GameRenderer();

	bool Initialize();

	void Update(double deltaTime);
	void Render(Scene* scene);
};

