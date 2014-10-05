#include "GameRenderer.h"

#include "Object handlers/TextureHandler.h"
#include "Object handlers/MaterialHandler.h"
#include "Object handlers/MeshHandler.h"
#include "Objects/Texture.h"

#include <easylogging++.h>

GameRenderer::GameRenderer()
{
	rotation = 0.0f;
}


GameRenderer::~GameRenderer()
{
}

bool GameRenderer::Initialize(MaterialHandler* mtlHandlerPtr, MeshHandler* meshHandlerPtr, TextureHandler* texHandlerPtr)
{
	materialHandler = mtlHandlerPtr;
	meshHandler = meshHandlerPtr;
	textureHandler = texHandlerPtr;

	glfwWindow = glfwGetCurrentContext();

	glfwGetFramebufferSize(glfwWindow, &screenWidth, &screenHeight);

	float ratio = (float)screenWidth / (float) screenHeight;
	glViewport(0, 0, screenWidth, screenHeight);
	glClearColor(0.4f, 0.4f, 0.5f, 1.0f);

	glm::vec3 camPos = glm::vec3(50.0f, 50.0f, 50.0f);
	glm::vec3 treePos = glm::vec3(0.0f, 0.0f, 0.0f);

	wvpMatrixStruct.projMatrix = glm::perspective(glm::radians(45.0f), ratio, 0.1f, 200.f);
	wvpMatrixStruct.modelMatrix = glm::translate(glm::mat4(), treePos);
	wvpMatrixStruct.viewMatrix = glm::lookAt(	
		camPos,
		treePos,
		glm::vec3(	0.0f, 0.0f, 1.0f	)
		);


	scaleMatrix = glm::scale(glm::mat4(), glm::vec3(0.1f, 0.1f, 0.1f));

	glGenBuffers(1, &matrixUBO);
	glBindBuffer(GL_UNIFORM_BUFFER, matrixUBO);
	glBufferData(GL_UNIFORM_BUFFER, sizeof(Matrices), (void *)(&wvpMatrixStruct), GL_DYNAMIC_DRAW);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);

	SetupRenderPasses();

	//Loading and compiling all shaders etc
	if(!shaderManager.Initialize())
	{
		return false;
	}

	Shader* genericShader = nullptr;
	if(!shaderManager.GetShader("objshader", &genericShader))
	{
		LOG(ERROR) << "Couldn't fetch objshader from shadermanager. Aborting.";
		return false;
	}
	else
	{
		objShader = static_cast<OBJShader*>(genericShader);
		objShader->SetupBuffers(matrixUBO, 0);
	}

	return true;
}

void GameRenderer::Update(double deltaTime)
{
	if(rotation > 360.0)
	{
		rotation -= 360.0;
	}

	rotation += deltaTime;

	//Updating view matrix to rotate...
	glm::mat4 rotMatrix
		(
		cos(rotation),		0.0f,	sin(rotation),		1.0f,
		0.0f,				1.0f,	0.0f,				1.0f,
		-sin(rotation),		0.0f,	cos(rotation),		1.0f,
		0.0f,				0.0f,	0.0f,				1.0f
		);

	glm::mat4x4 world = wvpMatrixStruct.modelMatrix * rotMatrix * scaleMatrix;

	glBindBuffer(GL_UNIFORM_BUFFER, matrixUBO);
	glBufferSubData(GL_UNIFORM_BUFFER, 0,						sizeof(glm::mat4),		glm::value_ptr(world));
	glBufferSubData(GL_UNIFORM_BUFFER, sizeof(glm::mat4),		sizeof(glm::mat4),		glm::value_ptr(wvpMatrixStruct.viewMatrix));
	glBufferSubData(GL_UNIFORM_BUFFER, sizeof(glm::mat4)*2,		sizeof(glm::mat4),		glm::value_ptr(wvpMatrixStruct.projMatrix));
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

void GameRenderer::Render(Scene* scene)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glDisable(GL_CULL_FACE);

	//This is where we render.
	for(unsigned int i = 0; i < renderPasses.size(); ++i)
	{
		renderPasses[i](scene);
	}
}

void GameRenderer::SetupRenderPasses()
{
	//So this might look weird. We store each render pass as a simple std::function
	std::function<void(Scene*)> objRenderPass
		(
		//Copy construct from a lambda
		[&](Scene* scene)
	{
		objShader->Enable();
		objShader->BindBuffers(matrixUBO, 0);

		
		Texture* currentTexture = nullptr;
		MaterialStruct* currentMaterial = nullptr;
		GLuint currentTextureKey = 0;
		GLuint currentMaterialKey = 0;
		auto& renderables = scene->renderables;

		for(unsigned int renderableIndex = 0; renderableIndex < renderables.size(); ++renderableIndex) 
		{
			auto& mesh = meshHandler->GetMesh(renderables[renderableIndex].meshHandle);
			auto& meshes = mesh.GetSubmeshes();

			/*
			! Update transform based on each model instance !?
			*/

			/*
			auto texKey = GetKey(meshes[j].textureIndex);
			if(currentTexture != texKey)
			{
			currentTexture = texKey;
			////Change texture.
			//glActiveTexture(GL_TEXTURE0);
			//int texture_location = glGetUniformLocation(fs, "color_texture");
			//glUniform1i(texture_location, 0);
			//glBindTexture(GL_TEXTURE_2D, texture[0]);

			}

			auto matKey = GetKey(meshes[j].materialIndex);
			if(currentMaterial != matKey)
			{
			currentMaterial = matKey;
			//Change material.
			}
			*/

			glBindVertexArray(mesh.vao);

			for(unsigned int submeshIndex = 0; submeshIndex < meshes.size(); ++submeshIndex)
			{
				glDrawElementsBaseVertex(GL_TRIANGLES, meshes[submeshIndex].numIndices, GL_UNSIGNED_INT,
										(void*)(sizeof(GLuint) * meshes[submeshIndex].baseIndex), meshes[submeshIndex].baseVertex);
			}

			glBindVertexArray(0);
		}

		objShader->Disable();
	}
	);


	//Then we insert each render pass in the same order that they will be executed
	renderPasses.push_back(objRenderPass);
}