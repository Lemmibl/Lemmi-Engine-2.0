#pragma once
#include "Camera.h"

#include <glew.h>
#include "../Core systems/Data classes/FlyweightHandle.h"
using namespace FlyweightHandleFunctions;

class Scene
{
//To access all the private goodies in here without a billion getters/setters.
//It's better encapsulation this way than just setting everything to public.
friend class GameRenderer;

public:
	Scene();
	~Scene();

	//In the future we'll probably have, like, a string to specify a level name or something along those lines. A random seed, maybe.
	void Load();

private:
	//void BindScene();

private:
	GLuint vertexBufferObject;
	GLuint vertexAttributeObject;
	Camera playerCamera;
	FlyweightHandle testHandle;

	float points[9];
};

