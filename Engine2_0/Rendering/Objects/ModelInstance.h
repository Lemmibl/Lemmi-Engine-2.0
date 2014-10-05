#pragma once
#include "../../Core systems/Data classes/FWHandle.h"
#include <glew.h>
#include <GLFW/glfw3.h>
#include <vector>

#include <glm/glm.hpp>

using namespace FlyweightFunctionality;

struct ModelInstance
{
	//Each submesh contains a mat & tex handle
	FWHandle meshHandle;

	//Actual transform for this mode.
	FWHandle transformHandle;
};
