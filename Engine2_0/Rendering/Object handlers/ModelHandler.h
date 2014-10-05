#pragma once
#include "HandlerBaseClass.h"
#include "../Objects/ModelInstance.h"

class MaterialHandler;
class MeshHandler;
class TextureHandler;

class ModelHandler : HandlerBaseClass<ModelInstance, unsigned short>
{
public:
	ModelHandler();
	~ModelHandler();

	bool Initialize(MaterialHandler* mtlHandlerPtr, MeshHandler* meshHandlerPtr, TextureHandler* texHandlerPtr);

	bool LoadModel(std::string fileName, FWHandle& outHandle);

	ModelInstance& GetModel(FWHandle handle);
	DODContainer<ModelInstance, unsigned short>& GetModelArray() { return objectContainer; }


private:
	MaterialHandler* materialHandler;
	MeshHandler* meshHandler; 
	TextureHandler* textureHandler;

	std::string baseFilepath;
};

