#pragma once

#include "Camera.h"

#include <glew.h>
#include "math_3d.h"

class Scene
{
//To access all the private goodies in here without a billion getters/setters.
//It's better encapsulation this way than just setting everything to public.
friend class GameRenderer;

public:
	Scene();
	~Scene();

	//In the future we'll probably have, like, a string to specify a level name or something along those lines. A random seed, maybe.
	void Load();

	GLuint GetVertexBuffer() { return vertexbuffer; }

private:
	void BindScene();

private:
	GLfloat vertices[9];
	GLuint vbo;

	// This will identify our vertex buffer
	GLuint vertexbuffer;

	Camera playerCamera;
};

