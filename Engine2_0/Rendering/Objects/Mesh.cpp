#include "Mesh.h"

Mesh::Mesh()
{
}

Mesh::~Mesh()
{
	if(buffers[POSITION_BUFFER] != 0) 
	{
		glDeleteBuffers(1, &buffers[POSITION_BUFFER]);
	}

	if(buffers[TEXCOORD_BUFFER] != 0) 
	{
		glDeleteBuffers(1, &buffers[TEXCOORD_BUFFER]);
	}

	if(buffers[NORMAL_BUFFER] != 0) 
	{
		glDeleteBuffers(1, &buffers[NORMAL_BUFFER]);
	}

	if(buffers[INDEX_BUFFER] != 0) 
	{
		glDeleteBuffers(1, &buffers[INDEX_BUFFER]);
	}

	glDeleteVertexArrays(1, &vao);

	submeshes.clear();
}

void Mesh::CreateMesh(const aiScene* scene)
{
	std::vector<GLuint> indices;
	std::vector<glm::vec3> positions;
	std::vector<glm::vec2> texCoords;
	std::vector<glm::vec3> normals;

	submeshes.resize(scene->mNumMeshes);

	unsigned int numVertices = 0;
	unsigned int numIndices = 0;

	// Count the number of vertices and indices
	for (unsigned int i = 0; i < submeshes.size(); ++i) 
	{
		submeshes[i].materialIndex = CreateHandle(HandleTypes::Texture, scene->mMeshes[i]->mMaterialIndex);
		submeshes[i].numIndices = scene->mMeshes[i]->mNumFaces * 3;
		submeshes[i].baseVertex = numVertices;
		submeshes[i].baseIndex = numIndices;

		numVertices += scene->mMeshes[i]->mNumVertices;
		numIndices += submeshes[i].numIndices;
	}

	// Reserve space in the vectors for the vertex attributes and indices
	indices.resize(numIndices);
	positions.resize(numVertices);
	normals.resize(numVertices);
	texCoords.resize(numVertices);


	unsigned int dataIndex = 0;

	//Fill it up with submeshes
	for(unsigned int i = 0; i < submeshes.size(); ++i) 
	{
		 const aiMesh* paiMesh = scene->mMeshes[i];

		LoadSubMesh(dataIndex, paiMesh, positions, normals, texCoords, indices);

		dataIndex += paiMesh->mNumVertices;
	}

	// Create the VAO
	glGenVertexArrays(1, &vao);
	
	glBindVertexArray(vao);

	// Create the buffers for the vertices atttributes
	glGenBuffers(bufferSize, buffers);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffers[INDEX_BUFFER]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices[0])*indices.size(), &indices[0], GL_STATIC_DRAW);


	// Generate and populate the buffers with vertex attributes and the indices
	glBindBuffer(GL_ARRAY_BUFFER, buffers[POSITION_BUFFER]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(positions[0]) * positions.size(), &positions[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);


	glBindBuffer(GL_ARRAY_BUFFER, buffers[NORMAL_BUFFER]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(normals[0]) * normals.size(), &normals[0],	GL_STATIC_DRAW);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);


	glBindBuffer(GL_ARRAY_BUFFER, buffers[TEXCOORD_BUFFER]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(texCoords[0]) * texCoords.size(), &texCoords[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, 0);

	//TODO: Try this?
	/*
	// Create the VAO
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);

	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, 0);

	glBindVertexArray(0);
	*/

	glBindVertexArray(0);
}

void Mesh::LoadSubMesh( unsigned int dataIndex, const aiMesh* paiMesh, std::vector<glm::vec3>& positions, std::vector<glm::vec3>& normals, 
		std::vector<glm::vec2>& texCoords, std::vector<GLuint>& indices )
{
	const aiVector3D Zero3D(0.0f, 0.0f, 0.0f);
	const aiVector3D defaultNormal(0.0f, 1.0f, 0.0f);

	// Populate the vertex attribute vectors
	for (unsigned int i = 0; i < paiMesh->mNumVertices; ++i) 
	{
		const aiVector3D* pPos =				&(paiMesh->mVertices[i]);
		const aiVector3D* pNormal =				paiMesh->HasNormals() ? &(paiMesh->mNormals[i]) : &defaultNormal;
		const aiVector3D* pTexCoord =			paiMesh->HasTextureCoords(0) ? &(paiMesh->mTextureCoords[0][i]) : &Zero3D;

		positions[dataIndex+i]	=		std::move(glm::vec3(pPos->x, pPos->y, pPos->z));
		normals[dataIndex+i]	=		std::move(glm::vec3(pNormal->x, pNormal->y, pNormal->z));
		texCoords[dataIndex+i]	=		std::move(glm::vec2(pTexCoord->x, pTexCoord->y));
	}

	unsigned int subIndex = dataIndex;

	// Populate the index buffer
	for (unsigned int i = 0; i < paiMesh->mNumFaces; ++i) 
	{
		const aiFace& Face = paiMesh->mFaces[i];

		indices[subIndex++]	= GLuint(Face.mIndices[0]);
		indices[subIndex++]	= GLuint(Face.mIndices[1]);
		indices[subIndex++]	= GLuint(Face.mIndices[2]);
	}
}

/*
// create array with faces
// have to convert from Assimp format to array
unsigned int *faceArray;
faceArray = (unsigned int *)malloc(sizeof(unsigned int) * mesh->mNumFaces * 3);
unsigned int faceIndex = 0;

for (unsigned int t = 0; t < mesh->mNumFaces; ++t) {
const aiFace* face = &mesh->mFaces[t];

memcpy(&faceArray[faceIndex], face->mIndices,3 * sizeof(unsigned int));
faceIndex += 3;
}
aMesh.numFaces = sc->mMeshes[n]->mNumFaces;

// generate Vertex Array for mesh
glGenVertexArrays(1,&(aMesh.vao));
glBindVertexArray(aMesh.vao);

// buffer for faces
glGenBuffers(1, &buffer);
glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffer);
glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * mesh->mNumFaces * 3, faceArray, GL_STATIC_DRAW);

// buffer for vertex positions
if (mesh->HasPositions()) {
glGenBuffers(1, &buffer);
glBindBuffer(GL_ARRAY_BUFFER, buffer);
glBufferData(GL_ARRAY_BUFFER, sizeof(float)*3*mesh->mNumVertices, mesh->mVertices, GL_STATIC_DRAW);
glEnableVertexAttribArray(vertexLoc);
glVertexAttribPointer(vertexLoc, 3, GL_FLOAT, 0, 0, 0);
}

// buffer for vertex normals
if (mesh->HasNormals()) {
glGenBuffers(1, &buffer);
glBindBuffer(GL_ARRAY_BUFFER, buffer);
glBufferData(GL_ARRAY_BUFFER, sizeof(float)*3*mesh->mNumVertices, mesh->mNormals, GL_STATIC_DRAW);
glEnableVertexAttribArray(normalLoc);
glVertexAttribPointer(normalLoc, 3, GL_FLOAT, 0, 0, 0);
}

// buffer for vertex texture coordinates
if (mesh->HasTextureCoords(0)) {
float *texCoords = (float *)malloc(sizeof(float)*2*mesh->mNumVertices);
for (unsigned int k = 0; k < mesh->mNumVertices; ++k) {

texCoords[k*2]   = mesh->mTextureCoords[0][k].x;
texCoords[k*2+1] = mesh->mTextureCoords[0][k].y; 

}
glGenBuffers(1, &buffer);
glBindBuffer(GL_ARRAY_BUFFER, buffer);
glBufferData(GL_ARRAY_BUFFER, sizeof(float)*2*mesh->mNumVertices, texCoords, GL_STATIC_DRAW);
glEnableVertexAttribArray(texCoordLoc);
glVertexAttribPointer(texCoordLoc, 2, GL_FLOAT, 0, 0, 0);
}

// unbind buffers
glBindVertexArray(0);
glBindBuffer(GL_ARRAY_BUFFER,0);
glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,0);
*/