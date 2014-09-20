#include "Scene.h"

Scene::Scene()
{
	testHandle = HandleFunctions::CreateHandle(HandleTypes::AI, 8000);

	unsigned int val = testHandle.key;
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

	points[0] = 0.0f;
	points[1] = 0.5f;
	points[2] = 0.0f;
	points[3] = 0.5f;
	points[4] = -0.5f;
	points[5] = 0.0f;
	points[6] = -0.5f;
	points[7] = -0.5f;
	points[8] = 0.0f;

	vertexBufferObject = 0;
	glGenBuffers(1, &vertexBufferObject);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObject);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float)*9, points, GL_STATIC_DRAW);

	vertexAttributeObject = 0;
	glGenVertexArrays (1, &vertexAttributeObject);
	glBindVertexArray (vertexAttributeObject);
	glEnableVertexAttribArray(0);
	glBindBuffer (GL_ARRAY_BUFFER, vertexBufferObject);
	glVertexAttribPointer (0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
}

//void Scene::BindScene()
//{
//	// Generate 1 buffer, put the resulting identifier in vertexbuffer
//	glGenBuffers(1, &vertexbuffer);
//
//	// The following commands will talk about our 'vertexbuffer' buffer
//	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
//
//	// Give our vertices to OpenGL.
//	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
//
//}
