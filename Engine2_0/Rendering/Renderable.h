#pragma once
#include "../Core systems/Data classes/FlyweightHandle.h"

struct Renderable
{
	unsigned short modelHandle;
	unsigned short shaderPasses; //One pass per bit..?
	unsigned int transformHandle;

};