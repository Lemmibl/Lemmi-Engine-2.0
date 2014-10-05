#include "MeshHandler.h"


MeshHandler::MeshHandler()
: HandlerBaseClass(64)
{
}


MeshHandler::~MeshHandler()
{
}


bool MeshHandler::LoadMesh(const aiScene* scene, std::string fileName, FWHandle& outHandle)
{
	FWHandle newHandle = 0;

	//If we've already loaded and saved this mesh before, we return the handle instead of repeating all the file reading
	if(LookForDuplicateObject(fileName, newHandle))
	{
		outHandle = newHandle;

		return true;
	}

	unsigned short meshHandle = 0;

	//Create a new mesh, get the handle to that mesh
	if(objectContainer.AddNewObject(meshHandle))
	{
		//Initialize object
		objectContainer[meshHandle].StoreMesh(scene);

		//Setup the handle properly
		newHandle = CreateHandle(HandleTypes::Mesh, meshHandle);

		//Save filename-handle pairing to make sure we don't load the same object several times
		InsertNewPair(fileName, newHandle);

		//Assign the right value to outhandle
		outHandle = newHandle;
		
		return true;
	}

	return false;
}

Mesh& MeshHandler::GetMesh( FWHandle meshHandle )
{
	return objectContainer.GetSpecificObject(FlyweightFunctionality::GetKey(meshHandle));
}
