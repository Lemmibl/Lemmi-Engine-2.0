#include "Mesh.h"

Mesh::Mesh()
{
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

Mesh::~Mesh()
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

void Mesh::StoreSubMesh(const aiMesh* mesh)
{
	SubMesh newSubMesh;

	newSubMesh.vbo[VERTEX_BUFFER] = NULL;
	newSubMesh.vbo[TEXCOORD_BUFFER] = NULL;
	newSubMesh.vbo[NORMAL_BUFFER] = NULL;
	newSubMesh.vbo[INDEX_BUFFER] = NULL;

	glGenVertexArrays(1, &newSubMesh.vao);
	glBindVertexArray(newSubMesh.vao);

	//Save element count
	newSubMesh.elementCount = mesh->mNumFaces * 3;

	//Save material index
	newSubMesh.materialIndex = mesh->mMaterialIndex;

	if(mesh->HasPositions()) 
	{
		std::vector<float> vertices(mesh->mNumVertices * 3);
		for(unsigned int i = 0; i < mesh->mNumVertices; ++i) 
		{
			vertices[i * 3] = mesh->mVertices[i].x;
			vertices[i * 3 + 1] = mesh->mVertices[i].y;
			vertices[i * 3 + 2] = mesh->mVertices[i].z;
		}

		glGenBuffers(1, &newSubMesh.vbo[VERTEX_BUFFER]);
		glBindBuffer(GL_ARRAY_BUFFER, newSubMesh.vbo[VERTEX_BUFFER]);
		glBufferData(GL_ARRAY_BUFFER, 3 * mesh->mNumVertices * sizeof(GLfloat), vertices.data(), GL_STATIC_DRAW);

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
		glEnableVertexAttribArray (0);
	}


	if(mesh->HasTextureCoords(0)) 
	{
		std::vector<float> texCoords(mesh->mNumVertices * 2);
		for(unsigned int i = 0; i < mesh->mNumVertices; ++i) 
		{
			texCoords[i * 2]		= mesh->mTextureCoords[0][i].x;
			texCoords[i * 2 + 1]	= mesh->mTextureCoords[0][i].y;
		}

		glGenBuffers(1, &newSubMesh.vbo[TEXCOORD_BUFFER]);
		glBindBuffer(GL_ARRAY_BUFFER, newSubMesh.vbo[TEXCOORD_BUFFER]);
		glBufferData(GL_ARRAY_BUFFER, 2 * mesh->mNumVertices * sizeof(GLfloat), texCoords.data(), GL_STATIC_DRAW);

		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, NULL);
		glEnableVertexAttribArray (1);
	}


	if(mesh->HasNormals()) 
	{
		std::vector<float> normals(mesh->mNumVertices * 3);

		for(unsigned int i = 0; i < mesh->mNumVertices; ++i)
		{
			normals[i * 3]		= mesh->mNormals[i].x;
			normals[i * 3 + 1]	= mesh->mNormals[i].y;
			normals[i * 3 + 2]	= mesh->mNormals[i].z;
		}

		glGenBuffers(1, &newSubMesh.vbo[NORMAL_BUFFER]);
		glBindBuffer(GL_ARRAY_BUFFER, newSubMesh.vbo[NORMAL_BUFFER]);
		glBufferData(GL_ARRAY_BUFFER, 3 * mesh->mNumVertices * sizeof(GLfloat), normals.data(), GL_STATIC_DRAW);

		glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, NULL);
		glEnableVertexAttribArray (2);
	}


	if(mesh->HasFaces()) 
	{
		std::vector<unsigned int> indices(mesh->mNumFaces * 3);
		for(unsigned int i = 0; i < mesh->mNumFaces; ++i) 
		{
			indices[i * 3]		= mesh->mFaces[i].mIndices[0];
			indices[i * 3 + 1]	= mesh->mFaces[i].mIndices[1];
			indices[i * 3 + 2]	= mesh->mFaces[i].mIndices[2];
		}

		glGenBuffers(1, &newSubMesh.vbo[INDEX_BUFFER]);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, newSubMesh.vbo[INDEX_BUFFER]);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, 3 * mesh->mNumFaces * sizeof(GLuint), indices.data(), GL_STATIC_DRAW);

		glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 0, NULL);
		glEnableVertexAttribArray (3);
	}


	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	submeshes.push_back(newSubMesh);
}

void Mesh::Render()
{
	//Maybe?
}

