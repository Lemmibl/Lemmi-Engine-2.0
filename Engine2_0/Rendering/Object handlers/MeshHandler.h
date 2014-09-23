#pragma once
#include <string>

#include "../../Core systems/Data classes/FlyweightHandle.h"
#include "../../Core systems/Data classes/DODArray.h"

#include "../Objects/Mesh.h"

using namespace HandleFunctions;

class MeshHandler
{
public:
	MeshHandler();
	~MeshHandler();

	FlyweightHandle LoadMesh(const aiScene* scene, std::string fileName);

	const Mesh& GetMesh(FlyweightHandle meshHandle);

private:
	bool LookForDuplicateMeshes(std::string fileName, FlyweightHandle& outHandle);
	void InsertNewPair(std::string filepath, FlyweightHandle handle);

private:
	DODContainer<Mesh, unsigned short> meshes;
	std::vector<std::pair<std::string, FlyweightHandle>> filePathAndMeshPairings;
};

/************************************************************************/
/* 

	Alright, so. Inside meshhandler we want to have a ptr to mathandler and a ptr to texhandler
	they will receive materials/textures from the loaded model, and insert them into their internal data structures.
	
	Each insert function will return a handle, which will be used to access that mat or texture.
	In the same way, when we load the model, the mesh will be inserted into a data structure here, and it will be accessed externally with a handle.

	How does the renderable come into the picture? I'm not sure yet. 
	Either we keep a renderable here and return it, or we keep meshes here, and when we've loaded the model we return a new renderable (by value), 
	containg all the different keys for different objects.


	advantage with the latter is that you can save the renderable somewhere, and copy it every time a new guy wants a new renderable..? No, that's stupid.
	
	Actually, maybe we just.. assemble a renderable everytime we call loadmodel? so it'll look through all the "has this already been loaded before?" and return all the relevant values...

																		*/
/************************************************************************/