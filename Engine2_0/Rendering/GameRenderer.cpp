#include "GameRenderer.h"

GameRenderer::GameRenderer()
{
}


GameRenderer::~GameRenderer()
{
}

bool GameRenderer::Initialize()
{
	glfwWindow = glfwGetCurrentContext();

	glfwGetFramebufferSize(glfwWindow, &screenWidth, &screenHeight);

	float ratio = (float)screenWidth / (float) screenHeight;
	glViewport(0, 0, screenWidth, screenHeight);
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

	SetupRenderPasses();

	//Loading and compiling all shaders etc
	if(!shaderManager.Initialize())
	{
		return false;
	}
	else
	{
		glUseProgram(shaderManager.GetShader(ShaderEnums::GeneralShader));
	}

	return true;
}

void GameRenderer::Update(double deltaTime)
{

}

void GameRenderer::Render(Scene* scene)
{
	glClearColor(0.4f, 0.4f, 0.5f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glDisable(GL_CULL_FACE);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-1, 1, -1, 1, 1.0, -1.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	//glBegin(GL_TRIANGLES);

	//This is where we render.
	for(unsigned int i = 0; i < renderPasses.size(); ++i)
	{
		renderPasses[i](scene);
	}

	//glEnd();
	glFlush();
}

void GameRenderer::SetupRenderPasses()
{
	//So this might look weird. We store each render pass as a simple std::function
	std::function<void(Scene*)> generalRenderPass
	(
		//Copy construct from a lambda
		[&](Scene* scene)
		{
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			GLuint shaderID = shaderManager.GetShader(ShaderEnums::GeneralShader);

			glUseProgram(shaderID);

			glBindVertexArray(scene->vertexAttributeObject);
			// draw points 0-3 from the currently bound VAO with current in-use shader
			glDrawArrays(GL_TRIANGLES, 0, 3);
		}
	);
	

	//Then we insert this render pass
	renderPasses.push_back(generalRenderPass);
}

/*
 
 void Mesh::Render()
 {
 glEnableVertexAttribArray(0);
 glEnableVertexAttribArray(1);
 glEnableVertexAttribArray(2);

 for (unsigned int i = 0 ; i < m_Entries.size() ; i++) {
 glBindBuffer(GL_ARRAY_BUFFER, m_Entries[i].VB);
 glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
 glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const GLvoid*)12);
 glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const GLvoid*)20);

 glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_Entries[i].IB);

 const unsigned int MaterialIndex = m_Entries[i].MaterialIndex;

 if (MaterialIndex < m_Textures.size() && m_Textures[MaterialIndex]) {
 m_Textures[MaterialIndex]->Bind(GL_TEXTURE0);
 }

 glDrawElements(GL_TRIANGLES, m_Entries[i].NumIndices, GL_UNSIGNED_INT, 0);
 }

 glDisableVertexAttribArray(0);
 glDisableVertexAttribArray(1);
 glDisableVertexAttribArray(2);
 }


 */