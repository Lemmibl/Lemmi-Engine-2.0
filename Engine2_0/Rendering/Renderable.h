#pragma once
struct Renderable
{
	unsigned short modelHandle;
	unsigned short shaderPasses; //One pass per bit..?
	unsigned int transformHandle;
};