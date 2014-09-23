#include "MeshHandler.h"


MeshHandler::MeshHandler()
: HandlerBaseClass(64)
{
}


MeshHandler::~MeshHandler()
{
}


FlyweightHandleFunctions::FlyweightHandle MeshHandler::LoadMesh(const aiScene* scene, std::string fileName)
{
	FlyweightHandle newHandle = 0;

	//If we've already loaded and saved this mesh before, we return the handle instead of repeating all the file reading
	if(LookForDuplicateObject(fileName, newHandle))
	{
		return newHandle;
	}

	unsigned short meshHandle = 0;

	//Create a new mesh, get the handle to that mesh
	if(objectContainer.AddNewObject(meshHandle))
	{
		//Fill it up with submeshes
		for(unsigned int i = 0; i < scene->mNumMeshes; ++i) 
		{
			objectContainer[meshHandle].StoreSubMesh(scene->mMeshes[i]);
		}

		newHandle = CreateHandle(HandleTypes::Mesh, meshHandle);

		InsertNewPair(fileName, newHandle);
	}

	return newHandle;
}

const Mesh& MeshHandler::GetMesh( FlyweightHandle meshHandle )
{
	return objectContainer.GetSpecificObject(FlyweightHandleFunctions::GetKey(meshHandle));
}
