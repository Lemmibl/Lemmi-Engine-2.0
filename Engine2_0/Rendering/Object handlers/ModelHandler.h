#pragma once
#include "../../Core systems/Data classes/DODArray.h"
#include "../../Core systems/Data classes/FlyweightHandle.h"
#include "../Objects/Model.h"

class MaterialHandler;
class MeshHandler;
class TextureHandler;

class ModelHandler
{
public:
	ModelHandler();
	~ModelHandler();

	bool Initialize(MaterialHandler* mtlHandlerPtr, MeshHandler* meshHandlerPtr, TextureHandler* texHandlerPtr);

	FlyweightHandle LoadModel(std::string fileName);
	const Model& GetModel(FlyweightHandle handle);
	DODContainer<Model, unsigned short>& GetModelArray() { return modelContainer; }

private:
	bool LookForDuplicateModels(std::string fileName, FlyweightHandle& outHandle);
	void InsertNewPair(std::string filepath, FlyweightHandle handle);

private:
	DODContainer<Model, unsigned short> modelContainer;
	std::vector<std::pair<std::string, FlyweightHandle>> filePathAndModelPairings;

	MaterialHandler* materialHandler;
	MeshHandler* meshHandler; 
	TextureHandler* textureHandler;
};

