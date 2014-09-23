#pragma once
#include "../../Core systems/Data classes/FlyweightHandle.h"
#include <glew.h>
#include <GLFW/glfw3.h>
#include <vector>

using namespace HandleFunctions;

class Model
{
public:
	Model();
	~Model();

	FlyweightHandle meshHandle;
	std::vector<FlyweightHandle> materialHandles;
	std::vector<FlyweightHandle> textureHandles;
};
