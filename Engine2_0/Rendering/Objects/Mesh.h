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
		GLint baseVertex;
		GLuint baseIndex;
		GLsizei numIndices;
		FWHandle materialIndex;
		FWHandle textureIndex;
	};

	enum SubMeshBufferIndex : GLuint
	{
		INDEX_BUFFER = 0,
		POSITION_BUFFER,
		NORMAL_BUFFER, 
		TEXCOORD_BUFFER
	};

public:
	Mesh();
	~Mesh();

	std::vector<SubMesh>& GetSubmeshes() { return submeshes; }

	void CreateMesh(const aiScene* scene);


private:
	void LoadSubMesh(unsigned int dataIndex, const aiMesh* paiMesh, std::vector<glm::vec3>& positions, std::vector<glm::vec3>& normals, 
	std::vector<glm::vec2>& texCoords, std::vector<GLuint>& indices);

private:
	static const unsigned int bufferSize = 4;
	GLuint buffers[bufferSize];

	GLuint vao;
	std::vector<SubMesh> submeshes;
};