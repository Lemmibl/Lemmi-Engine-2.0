#include "GameRenderer.h"

#include "../Game/Scene.h"

GameRenderer::GameRenderer()
{
}


GameRenderer::~GameRenderer()
{
}

bool GameRenderer::Initialize()
{
	//Initialize GLEW
	GLenum res = glewInit();
	if (res != GLEW_OK)
	{
		fprintf(stderr, "Error: '%s'\n", glewGetErrorString(res));
		return false;
	}

	glfwWindow = glfwGetCurrentContext();

	glfwGetFramebufferSize(glfwWindow, &screenWidth, &screenHeight);

	float ratio = (float)screenWidth / (float) screenHeight;
	glViewport(0, 0, screenWidth, screenHeight);
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

	SetupRenderPasses();

	return true;
}

void GameRenderer::Update(double deltaTime)
{

}

void GameRenderer::Render(Scene* scene)
{
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-ratio, ratio, -1.f, 1.f, 1.f, -1.f);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glClear(GL_COLOR_BUFFER_BIT);
	glBegin(GL_TRIANGLES);

	//This is where we render.
	for(unsigned int i = 0; i < renderPasses.size(); ++i)
	{
		renderPasses[i](scene);
	}

	glEnd();
}

void GameRenderer::SetupRenderPasses()
{
	//So this might look weird. We store each render pass as a simple std::function
	std::function<void(Scene*)> generalRenderPass
	(
		//Copy construct from a lambda
		[&](Scene* scene)
		{
			// 1rst attribute buffer : vertices
			glEnableVertexAttribArray(0);
			glBindBuffer(GL_ARRAY_BUFFER, scene->vertexbuffer);
			glVertexAttribPointer(
				0,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
				3,                  // size
				GL_FLOAT,           // type
				GL_FALSE,           // normalized?
				0,                  // stride
				(void*)0            // array buffer offset
				);

			// Draw the triangle !
			glDrawArrays(GL_TRIANGLES, 0, 3); // Starting from vertex 0; 3 vertices total -> 1 triangle

			glDisableVertexAttribArray(0);
		}
	);
	

	//Then we insert this render pass
	renderPasses.push_back(generalRenderPass);
}
