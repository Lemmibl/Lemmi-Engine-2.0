#pragma once
#include <windows.h>
#include <glew.h>
#include <GLFW/glfw3.h>

#include <vector>
#include "Camera.h"
#include "../Core systems/Data classes/FWHandle.h"
#include "../Rendering/Objects/ModelInstance.h"

using namespace FlyweightFunctionality;

class ModelHandler;

class Scene
{
//To access all the private goodies in here without a billion getters/setters.
//It's better encapsulation this way than just setting everything to public.
friend class GameRenderer;

public:
	Scene();
	~Scene();

	//In the future we'll probably have, like, a string to specify a level name or something along those lines. A random seed, maybe.
	void Load(ModelHandler* modelHandler);

	void Update(double deltaTime);

private:
	GLuint vertexBufferObject;
	GLuint vertexAttributeObject;
	Camera playerCamera;
	ModelHandler* modelHandler;

	std::vector<ModelInstance> renderables;

	float points[9];
};

