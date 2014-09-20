#pragma once
#include "../Core systems/Data classes/FlyweightHandle.h"
#include "RenderableModel.h"
#include "assimp/scene.h"
#include "assimp/mesh.h"

using namespace HandleFunctions;

class MeshHandler
{
public:
	MeshHandler();
	~MeshHandler();

	FlyweightHandle LoadMesh(const char* fileName);

private:
	void LoadSubMesh(aiMesh* subMesh);

private:
	//DODContainer for meshes
	//vector or whatever for the std::pair<string, handle>
};

