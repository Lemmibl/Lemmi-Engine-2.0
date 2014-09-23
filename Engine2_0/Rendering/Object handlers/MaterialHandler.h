#pragma once
#include <glew.h>
#include <GLFW/glfw3.h>
#include <assimp/scene.h>

#include <string>
#include <vector>

#include "../../Core systems/Data classes/DODArray.h"
#include "../../Core systems/Data classes/FlyweightHandle.h"

#include "MaterialStruct.h"

using namespace HandleFunctions;

class MaterialHandler
{
public:
	MaterialHandler();
	~MaterialHandler();

	FlyweightHandle LoadMaterial(std::string filename, aiMaterial* material);

private:
	bool LookForDuplicateMaterial(std::string fileName, FlyweightHandle& outHandle);
	void InsertNewPair(std::string filename, FlyweightHandle handle);

private:
	DODContainer<MaterialStruct, unsigned short> materialContainer;
	std::vector<std::pair<std::string, FlyweightHandle>> fileNameAndMaterialPairings;
};

