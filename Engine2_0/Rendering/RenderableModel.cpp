#include "RenderableModel.h"

RenderableModel::RenderableModel(unsigned short extSubMeshCount)
{
	submeshes.resize(extSubMeshCount);
	textureHandles.resize(extSubMeshCount);
	materialHandles.resize(extSubMeshCount);

	for(unsigned short i = 0; i < submeshes.size(); ++i)
	{
		auto& arrayElement(submeshes[i]);
		
		arrayElement.elementCount = 0;
		arrayElement.vao = 0;	
		arrayElement.vbo[VERTEX_BUFFER] = 0;
		arrayElement.vbo[TEXCOORD_BUFFER] = 0;
		arrayElement.vbo[NORMAL_BUFFER] = 0;
		arrayElement.vbo[INDEX_BUFFER] = 0;
	}
}

RenderableModel::~RenderableModel()
{
	for(unsigned short i = 0; i < submeshes.size(); ++i)
	{
		auto& arrayElement(submeshes[i]);

		if(arrayElement.vbo[VERTEX_BUFFER] != 0) 
		{
			glDeleteBuffers(1, &arrayElement.vbo[VERTEX_BUFFER]);
		}

		if(arrayElement.vbo[TEXCOORD_BUFFER] != 0) 
		{
			glDeleteBuffers(1, &arrayElement.vbo[TEXCOORD_BUFFER]);
		}

		if(arrayElement.vbo[NORMAL_BUFFER] != 0) 
		{
			glDeleteBuffers(1, &arrayElement.vbo[NORMAL_BUFFER]);
		}

		if(arrayElement.vbo[INDEX_BUFFER] != 0) 
		{
			glDeleteBuffers(1, &arrayElement.vbo[INDEX_BUFFER]);
		}

		glDeleteVertexArrays(1, &arrayElement.vao);

		arrayElement.vao = 0;
		arrayElement.elementCount = 0;
	}

	submeshes.clear();
}