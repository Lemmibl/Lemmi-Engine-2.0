#pragma once
#include "../Core systems/Data classes/FlyweightHandle.h"
#include <glew.h>
#include <GLFW/glfw3.h>

#include <vector>

using namespace HandleFunctions;

//This struct isn't nested within Mesh because it's used directly when rendering. Bad? Idk. Maybe. Might be up for debate with.. myself. In the future.
struct SubMesh 
{
	GLuint vbo[4];
	GLuint vao;
	unsigned int elementCount;
};

class RenderableModel
{
private:
	enum SubMeshBufferIndex
	{
		VERTEX_BUFFER = 0, 
		TEXCOORD_BUFFER, 
		NORMAL_BUFFER, 
		INDEX_BUFFER
	};

public:
	RenderableModel(unsigned short subMeshCount);
	~RenderableModel();

	std::vector<SubMesh> submeshes;
	std::vector<FlyweightHandle> materialHandles;
	std::vector<FlyweightHandle> textureHandles;
};
