#pragma once
#include <glew.h>
#include <GLFW/glfw3.h>
#include <assimp/scene.h>
#include "../Objects/Mesh.h"
#include "HandlerBaseClass.h"

class MaterialHandler;
class TextureHandler;

class MeshHandler : HandlerBaseClass<Mesh, unsigned short>
{
public:
	MeshHandler();
	~MeshHandler();

	bool LoadOBJMesh(MaterialHandler* materialHandler, TextureHandler* textureHandler, std::string fileName, FWHandle& outHandle);
	Mesh& GetMesh(FWHandle meshHandle);
};