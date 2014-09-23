#pragma once
#include <glew.h>
#include <GLFW/glfw3.h>
#include <vector>

#include "assimp/scene.h"
#include "assimp/mesh.h"


class Mesh
{
public:
	Mesh();
	~Mesh();

	void StoreSubMesh(const aiMesh* mesh);
	void Render();

private:
	struct SubMesh 
	{
		GLuint vbo[4];
		GLuint vao;
		unsigned int elementCount;
		unsigned int materialIndex;
	};

	enum SubMeshBufferIndex
	{
		VERTEX_BUFFER = 0, 
		TEXCOORD_BUFFER, 
		NORMAL_BUFFER, 
		INDEX_BUFFER
	};

private:
	std::vector<SubMesh> submeshes;
};