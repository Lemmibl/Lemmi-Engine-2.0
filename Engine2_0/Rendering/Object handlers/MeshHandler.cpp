#include "MeshHandler.h"


MeshHandler::MeshHandler()
: meshes(64)
{
}


MeshHandler::~MeshHandler()
{
}


HandleFunctions::FlyweightHandle MeshHandler::LoadMesh(const aiScene* scene, std::string fileName)
{
	FlyweightHandle newHandle;

	//If we've already loaded and saved this mesh before, we return the handle instead of repeating all the file reading
	if(LookForDuplicateMeshes(fileName, newHandle))
	{
		return newHandle;
	}

	unsigned short meshHandle = 0;

	//Create a new mesh, get the handle to that mesh
	if(meshes.AddNewObject(meshHandle))
	{
		//Get reference to it
		Mesh& newMesh = meshes.GetSpecificObject(meshHandle);

		//Fill it up with submeshes
		for(unsigned int i = 0; i < scene->mNumMeshes; ++i) 
		{
			newMesh.StoreSubMesh(scene->mMeshes[i]);
		}

		newHandle = CreateHandle(HandleTypes::Mesh, meshHandle);

		InsertNewPair(fileName, newHandle);
	}

	return newHandle;
}

bool MeshHandler::LookForDuplicateMeshes( std::string fileName, FlyweightHandle& outHandle )
{
	//For each mesh that we've stored in our DODArray
	for(unsigned int i = 0; i < filePathAndMeshPairings.size(); ++i)
	{
		//If the filename matches  any of our stored filenames...
		if(filePathAndMeshPairings[i].first == fileName)
		{
			//We return the handle to that object
			outHandle = filePathAndMeshPairings[i].second;

			//And return true to indicate that we've succeeded
			return true;
		}
	}

	//We return false if we haven't saved this filename before
	return false;
}

void MeshHandler::InsertNewPair(std::string filepath, FlyweightHandle handle)
{
	filePathAndMeshPairings.push_back(std::make_pair<std::string, FlyweightHandle>(filepath, handle));
}

const Mesh& MeshHandler::GetMesh( FlyweightHandle meshHandle )
{
	return meshes.GetSpecificObject(HandleFunctions::GetKey(meshHandle));
}
