#include "ModelHandler.h"

#include "TextureHandler.h"
#include "MaterialHandler.h"
#include "MeshHandler.h"
#include "TransformHandler.h"

#include <string>
#include <algorithm> //For ToLower function on string

#include <easylogging++.h>

ModelHandler::ModelHandler()
	: HandlerBaseClass(512)
{
}


ModelHandler::~ModelHandler()
{
}

bool ModelHandler::Initialize(MaterialHandler* mtlHandlerPtr, MeshHandler* meshHandlerPtr, TextureHandler* texHandlerPtr, TransformHandler* transformHandlerPtr)
{
	materialHandler = mtlHandlerPtr;
	meshHandler = meshHandlerPtr;
	textureHandler = texHandlerPtr;
	transformHandler = transformHandlerPtr;

	return true;
}

FWHandle ModelHandler::CreateModelInstance(std::string meshName, glm::vec3 position, glm::vec3 rotation, glm::vec3 scale)
{
	//Just to enforce uniformity I make all characters lowercase.
	//I need uniformity to make sure that string comparisons don't fail because of upper/lowercase inconsistencies.
	std::transform(meshName.begin(), meshName.end(), meshName.begin(), ::tolower);

	//Potential error returns it's supposed to be an invalid handle.
	FWHandle returnHandle = HandleTypes::InvalidHandle;

	////If there's a duplicate, return the key to that one instead
	//if(LookForDuplicateObject(meshName, returnHandle))
	//{
	//	//If we go here, we don't return a proper transform
	//	return returnHandle;
	//}


	unsigned short objectKey = 0;

	//If there's an object slot available
	if(objectContainer.AddNewObject(objectKey))
	{
		ModelInstance* modelPtr = &objectContainer[objectKey];

		//Create a mesh and write everything to modelPtr
		if(SetupMesh(meshName, modelPtr))
		{
			returnHandle = CreateHandle(HandleTypes::Model, objectKey);

			//Don't need this, each model will have a unique transform
			////Make sure we remember that we've loaded this model so that we don't load the same file several times
			//InsertNewPair(meshName, returnHandle);
		
			//Create a transform and write the key to modelPtr
			SetupTransform(position, rotation, scale, modelPtr);

			return returnHandle;
		}
		else
		{
			LOG(ERROR) << "Couldn't setup model mesh.";
		}
	}
	else
	{
		LOG(ERROR) << "Couldn't load model: no more room in model container.";
	}

	return returnHandle;
}

ModelInstance& ModelHandler::GetModel(FWHandle handle)
{
	return objectContainer[FlyweightFunctionality::GetKey(handle)];
}

bool ModelHandler::SetupMesh(std::string meshName, ModelInstance* outModel)
{
	//Load the mesh and get the handle back...
	FWHandle meshHandle;
	if(!meshHandler->LoadOBJMesh(materialHandler, textureHandler, meshName, outModel->meshHandle))
	{
		LOG(ERROR) << "Could not load mesh by the name of: " + meshName;
		return false;
	}

	return true;
}

void ModelHandler::SetupTransform(glm::vec3 position, glm::vec3 rotation, glm::vec3 scale, ModelInstance* outModel)
{
	outModel->transformHandle = transformHandler->CreateTransform(position, rotation, scale);
}