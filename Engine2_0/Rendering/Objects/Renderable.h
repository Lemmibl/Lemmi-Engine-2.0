#pragma once
#include "../../Core systems/Data classes/FlyweightHandle.h"

using namespace HandleFunctions;

struct Renderable
{
	//This describes what passes we should be rendering this renderable
	unsigned short renderPasses; //One pass per bit..?

	//Handle to the transform for this renderable
	FlyweightHandle transformHandle;

	//And handle to the actual model we'll be rendering
	FlyweightHandle modelHandle;
};