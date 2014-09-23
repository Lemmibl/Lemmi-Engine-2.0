#include "ModelHandler.h"

#include "TextureHandler.h"
#include "MaterialHandler.h"
#include "MeshHandler.h"

#include <string>
#include <algorithm> //For ToLower function on string

#include <assimp/scene.h>
#include <assimp/Importer.hpp>

static const std::string baseFilepath = "../Engine2_0/Data/Models/";

ModelHandler::ModelHandler()
: modelContainer(64)
{
}


ModelHandler::~ModelHandler()
{
}

bool ModelHandler::Initialize(MaterialHandler* mtlHandlerPtr, MeshHandler* meshHandlerPtr, TextureHandler* texHandlerPtr)
{
	materialHandler = mtlHandlerPtr;
	meshHandler = meshHandlerPtr;
	textureHandler = texHandlerPtr;

	return true;
}

FlyweightHandleFunctions::FlyweightHandle ModelHandler::LoadModel(std::string fileName)
{
	//Just to enforce uniformity I force all characters to lowercase.
	//I need uniformity to make sure that string comparisons don't fail because of upper/lowercase inconsistencies.
	std::transform(fileName.begin(), fileName.end(), fileName.begin(), ::tolower);

	FlyweightHandle returnHandle;

	//If there's a duplicate, return the key to that one instead
	if(LookForDuplicateModels(fileName, returnHandle))
	{
		return returnHandle;
	}

	//Import model file
	Assimp::Importer loader;
		
	auto scene = loader.ReadFile(baseFilepath+fileName, 0);

	//These will be used to cross reference the model's internal mtl/tex handles with the real engine handles for the same. Will probably be deleted at the end.
	std::vector<FlyweightHandle> materials(scene->mNumMaterials);
	std::vector<FlyweightHandle> textures(scene->mNumTextures);

	FlyweightHandle meshHandle = meshHandler->LoadMesh(scene, fileName);

	//for(each mesh, each material, each texture) call Insert/load into the right handler
	//in the case of material/texture, match the internal model handles with external handles so 
	// that when the model wants to access "texture 01" it has to correspond to the right handle in texture handler


	//Make sure we remember that we've loaded this model so that we don't load the same file several times
	InsertNewPair(fileName, returnHandle);

	return returnHandle;
}

const Model& ModelHandler::GetModel(FlyweightHandle handle)
{
	return modelContainer.GetSpecificObject(FlyweightHandleFunctions::GetKey(handle));
}

bool ModelHandler::LookForDuplicateModels(std::string fileName, FlyweightHandle& outHandle)
{
	//For each model that we've stored in our DODArray
	for(unsigned int i = 0; i < filePathAndModelPairings.size(); ++i)
	{
		//If the filename matches any of the stored filenames...
		if(filePathAndModelPairings[i].first == fileName)
		{
			//We return the handle to that object
			outHandle = filePathAndModelPairings[i].second;

			//And return true to indicate that we've succeeded
			return true;
		}
	}

	//We return false if we haven't saved this filename before
	return false;
}

void ModelHandler::InsertNewPair(std::string filepath, FlyweightHandle handle)
{
	filePathAndModelPairings.push_back(std::make_pair<std::string, FlyweightHandle>(filepath, handle));
}
