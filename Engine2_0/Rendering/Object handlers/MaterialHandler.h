#pragma once
#include <glew.h>
#include <GLFW/glfw3.h>
#include <assimp/scene.h>

#include "MaterialStruct.h"
#include "HandlerBaseClass.h"

class MaterialHandler : HandlerBaseClass<MaterialStruct, unsigned short>
{
public:
	MaterialHandler();
	~MaterialHandler();

	FlyweightHandle LoadMaterial(std::string filename, aiMaterial* material);
};

