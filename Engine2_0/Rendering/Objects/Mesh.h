#pragma once
#include <glew.h>
#include <GLFW/glfw3.h>
#include <assimp/scene.h>
#include <assimp/mesh.h>
#include <glm/glm.hpp>

#include <vector>

#include "../../Core systems/Data classes/FWHandle.h"

using namespace FlyweightFunctionality;

class Mesh
{
friend class GameRenderer;

private:
	struct SubMesh 
	{
		unsigned int baseVertex;
		unsigned int baseIndex;
		unsigned int numIndices;
		FWHandle materialIndex;
		FWHandle textureIndex;
	};

	enum SubMeshBufferIndex : GLuint
	{
		POSITION_BUFFER = 0, 
		NORMAL_BUFFER, 
		TEXCOORD_BUFFER,
		INDEX_BUFFER
	};

public:
	Mesh();
	~Mesh();

	std::vector<SubMesh>& GetSubmeshes() { return submeshes; }

	void StoreMesh(const aiScene* scene);


private:
	void LoadSubMesh(	const aiMesh* paiMesh, std::vector<glm::vec3>& positions, std::vector<glm::vec3>& normals, 
						std::vector<glm::vec2>& texCoords, std::vector<unsigned int>& indices);

private:
	static const unsigned int bufferSize = 4;

	GLuint vao;
	GLuint buffers[bufferSize];
	std::vector<SubMesh> submeshes;
};