#include "MeshHandler.h"

#include "MaterialHandler.h"
#include "TextureHandler.h"
#include <easylogging++.h>

#include <assimp/scene.h>
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/material.h>

static const std::string baseFilepath("../Engine2_0/Data/Models/");

MeshHandler::MeshHandler()
	: HandlerBaseClass(64)
{
}


MeshHandler::~MeshHandler()
{
}


bool MeshHandler::LoadOBJMesh(MaterialHandler* materialHandler, TextureHandler* textureHandler, std::string fileName, FWHandle& outHandle)
{
	FWHandle newHandle = 0;

	//If we've already loaded and saved this mesh before, we return the handle instead of repeating all the file reading
	if(LookForDuplicateObject(fileName, newHandle))
	{
		outHandle = newHandle;

		return true;
	}

	//Import model file
	Assimp::Importer loader;

	//Load the model file with assimp
	const aiScene* scene = loader.ReadFile(baseFilepath + fileName, aiProcess_FlipWindingOrder|aiProcess_Triangulate|aiProcess_FlipUVs); // | aiProcess_GenSmoothNormals | 

	if(nullptr != scene)
	{
		unsigned short meshHandle = 0;

		//Create a new mesh, get the handle to that mesh
		if(objectContainer.AddNewObject(meshHandle))
		{
			//Initialize object
			objectContainer[meshHandle].CreateMesh(scene);

			//These will be used to cross reference the model's internal mtl/tex handles with the real engine handles for the same. Will probably be deleted at the end.
			std::vector<std::pair<FWHandle, FWHandle>> textureMaterialPairings(scene->mNumMaterials);

			//We look for textures in each material, as well as loading in material data from each
			for(unsigned int i = 0; i < scene->mNumMaterials; ++i)
			{
				//Worst case scenario: we fail to load materialname. If that happens, we just load the default material and use that.
				std::string materialName("DefaultMaterial");

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
					//If we succeeded fetching the texture name from the material, we copy materialname into our string
					materialName = tempMatName.C_Str();
				}

				FWHandle materialHandle;
				if(!materialHandler->LoadMaterial(scene->mMaterials[i], materialName, materialHandle))
				{
					LOG(ERROR) << "Could not load material when trying to load file: " + fileName;
				}

				/*
				In the future: For each texture, inside each material.
				*/

				FWHandle textureHandle;
				if(!textureHandler->LoadTextureAssimp(scene->mMaterials[i], materialName, 0, textureHandle))
				{
					LOG(ERROR) << "Could not load texture when trying to load file: " + fileName;
				}

				textureMaterialPairings[i] = std::make_pair(textureHandle, materialHandle);
			}

			//Match mesh material ids with flyweight handles
			auto& submeshes = objectContainer[meshHandle].GetSubmeshes();

			//For each submesh, we need to update the material index to reflect reality
			for(unsigned int i = 0; i < submeshes.size(); ++i)
			{
				//Fetch the local cross referencing index the mesh format uses to keep track of its' materials
				unsigned int localIndex = GetKey(submeshes[i].materialIndex);

				//And update the index to reflect the handle values that we use
				submeshes[i].textureIndex = textureMaterialPairings[localIndex].first;
				submeshes[i].materialIndex = textureMaterialPairings[localIndex].second;
			}



			//Setup the handle properly
			newHandle = CreateHandle(HandleTypes::Mesh, meshHandle);

			//Save filename-handle pairing to make sure we don't load the same object several times
			InsertNewPair(fileName, newHandle);

			//Assign the right value to outhandle
			outHandle = newHandle;

			return true;
		}
	}

	return false;
}

Mesh& MeshHandler::GetMesh(FWHandle meshHandle)
{
	return objectContainer.GetSpecificObject(FlyweightFunctionality::GetKey(meshHandle));
}
