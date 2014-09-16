#include "Scene.h"


Scene::Scene()
{
	
}


Scene::~Scene()
{
}

void Scene::Load()
{
	//We allocate a GLuint in the global part of the program to store the handle of the vertex buffer object. 
	//You will see later that most (if not all) OpenGL objects are accessed via a variable of GLuint type.

	/*OpenGL defines several glGen* functions for generating objects of various types. 
	They often take two parameters - the first one specifies the number of objects you want 
	to create and the second is the address of an array of GLuints to store the handles that 
	the driver allocates for you (make sure the array is large enough to handle your request!). 

	Future calls to this function will not generate the same object handles unless you delete them first 
	with glDeleteBuffers. Note that at this point you don't specify what you intend to do with 
	the buffers so they can be regarded as "generic". This is the job of the next function.
	*/

	GLuint VertexArrayID;
	glGenVertexArrays(1, &VertexArrayID);
	glBindVertexArray(VertexArrayID);

	vertices[0] = -1.0f;
	vertices[1] = -1.0f;
	vertices[2] = 0.0f;
	vertices[3] = 1.0f;
	vertices[4] = -1.0f;
	vertices[5] = 0.0f;
	vertices[6] = 0.0f;
	vertices[7] = 1.0f;
	vertices[8] = 0.0f;

	BindScene();
}

void Scene::BindScene()
{
	// Generate 1 buffer, put the resulting identifier in vertexbuffer
	glGenBuffers(1, &vertexbuffer);

	// The following commands will talk about our 'vertexbuffer' buffer
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);

	// Give our vertices to OpenGL.
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

}
