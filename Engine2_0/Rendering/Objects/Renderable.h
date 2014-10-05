#pragma once
#include "../../Core systems/Data classes/FWHandle.h"

using namespace HandleFunctions;

struct Renderable
{
	//This describes what passes we should be rendering this renderable
	unsigned short renderPasses; //One pass per bit..?

	//Handle to the transform for this renderable
	FWHandle transformHandle;

	//And handle to the actual model we'll be rendering
	FWHandle modelHandle;
};