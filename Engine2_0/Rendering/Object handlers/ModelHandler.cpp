#include "ModelHandler.h"

#include "TextureHandler.h"
#include "MaterialHandler.h"
#include "MeshHandler.h"

#include <string>
#include <algorithm> //For ToLower function on string

#include <assimp/scene.h>
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/material.h>

#include <easylogging++.h>

ModelHandler::ModelHandler()
	: HandlerBaseClass(64),
	baseFilepath("../Engine2_0/Data/Models/")
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

bool ModelHandler::LoadModel(std::string fileName, FWHandle& outHandle)
{
	//Just to enforce uniformity I force all characters to lowercase.
	//I need uniformity to make sure that string comparisons don't fail because of upper/lowercase inconsistencies.
	std::transform(fileName.begin(), fileName.end(), fileName.begin(), ::tolower);

	FWHandle returnHandle;

	//If there's a duplicate, return the key to that one instead
	if(LookForDuplicateObject(fileName, returnHandle))
	{
		outHandle = returnHandle;
		return true;
	}

	unsigned short objectKey = 0;

	if(objectContainer.AddNewObject(objectKey))
	{
		//Import model file
		Assimp::Importer loader;

		const aiScene* scene = loader.ReadFile((baseFilepath+fileName), aiProcess_Triangulate|aiProcess_FlipUVs); // | aiProcess_GenSmoothNormals | 

		if(scene != nullptr)
		{
			////These will be used to cross reference the model's internal mtl/tex handles with the real engine handles for the same. Will probably be deleted at the end.
			std::vector<std::pair<FWHandle, FWHandle>> textureMaterialPairings(scene->mNumMaterials);

			//Load the mesh and get the handle back...
			FWHandle meshHandle;
			if(!meshHandler->LoadMesh(scene, fileName, meshHandle))
			{
				LOG(ERROR) << "Could not load mesh by the name of: " + fileName;
			}

			//We look for textures in each material, as well as loading in material data from each
			for(unsigned int i = 0; i < scene->mNumMaterials; ++i)
			{
				//Worst case scenario: we fail to load materialname. If that happens, we just load the default material and use that.
				std::string stdMatName("DefaultMaterial");

				//Temp string to hold the value we're fetching
				aiString tempMatName;

				//Fetch material name
				aiReturn result = scene->mMaterials[i]->Get(AI_MATKEY_NAME, tempMatName);
				if(AI_SUCCESS != result)
				{
					LOG(ERROR) << "Couldn't fetch material name from file by the name of: " + fileName;
				}
				else
				{
					//Copy materialname into our std string
					stdMatName = tempMatName.C_Str();
				}
				
				FWHandle materialHandle;
				if(!materialHandler->LoadMaterial(scene->mMaterials[i], stdMatName, materialHandle))
				{
					LOG(ERROR) << "Could not load material when trying to load file: " + fileName;
				}

				/*
				In the future: For each texture, inside each material.
				*/

				FWHandle textureHandle;
				if(!textureHandler->LoadTextureAssimp(scene->mMaterials[i], fileName, 0, textureHandle))
				{
					LOG(ERROR) << "Could not load texture when trying to load file: " + fileName;
				}

				textureMaterialPairings[i] = std::make_pair<FWHandle, FWHandle>(textureHandle, materialHandle);
			}

			//Match mesh material ids with flyweight handles
			auto& submeshes = meshHandler->GetMesh(meshHandle);

			//For each submesh, we need to update the material index to reflect reality
			for(unsigned int i = 0; i < submeshes.GetSubmeshes().size(); ++i)
			{
				//Fetch the local cross referencing index the mesh format uses to keep track of its' materials
				unsigned int localIndex = GetKey(submeshes.GetSubmeshes()[i].materialIndex);

				//And update the index to reflect the handle values that we use
				submeshes.GetSubmeshes()[i].textureIndex = textureMaterialPairings[localIndex].first;
				submeshes.GetSubmeshes()[i].materialIndex = textureMaterialPairings[localIndex].second;
			}


			//Save away the data
			objectContainer[objectKey].meshHandle = meshHandle;

			returnHandle = CreateHandle(HandleTypes::Model, objectKey);

			//Make sure we remember that we've loaded this model so that we don't load the same file several times
			InsertNewPair(fileName, returnHandle);

			outHandle = returnHandle;
		}
		else
		{
			LOG(ERROR) << "Couldn't load model file by the name of: " + fileName;
			return false;
		}
	}
	else
	{
		LOG(ERROR) << "Couldn't load model: no more room in model container.";
	}

	return true;
}

ModelInstance& ModelHandler::GetModel(FWHandle handle)
{
	return objectContainer[FlyweightFunctionality::GetKey(handle)];
}