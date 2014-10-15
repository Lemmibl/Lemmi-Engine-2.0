#pragma once
#include "../../Core systems/Data classes/FWHandle.h"
#include <glew.h>
#include <GLFW/glfw3.h>
#include <vector>

#include <glm/glm.hpp>

using namespace FlyweightFunctionality;

struct ModelInstance
{
	FWHandle transformHandle;
	FWHandle meshHandle;
};
