#pragma once
#include "HandlerBaseClass.h"
#include "../Objects/ModelInstance.h"

class MaterialHandler;
class MeshHandler;
class TextureHandler;
class TransformHandler;

class ModelHandler : HandlerBaseClass<ModelInstance, unsigned short>
{
public:
	ModelHandler();
	~ModelHandler();

	bool Initialize(MaterialHandler* mtlHandlerPtr, MeshHandler* meshHandlerPtr, TextureHandler* texHandlerPtr, TransformHandler* transformHandlerPtr);

	FWHandle CreateModelInstance(std::string meshName, glm::vec3 position, glm::vec3 rotation, glm::vec3 scale);

	ModelInstance& GetModel(FWHandle handle);
	DODContainer<ModelInstance, unsigned short>& GetModelArray() { return objectContainer; }

private:
	bool SetupMesh(std::string meshName, ModelInstance* outModel);
	void SetupTransform(glm::vec3 position, glm::vec3 rotation, glm::vec3 scale, ModelInstance* outModel);

private:
	MaterialHandler* materialHandler;
	MeshHandler* meshHandler; 
	TextureHandler* textureHandler;
	TransformHandler* transformHandler;
};

