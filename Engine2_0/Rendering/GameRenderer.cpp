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

	glm::vec3 camPos(0.0f, 25.0f, -40.0f);
	glm::vec3 treePos(0.0f, 0.0f, 0.0f);
	glm::vec3 targetOffset(0.0f, 1.0f, 0.0f);
	glm::vec3 scalingVector(0.7f);

	scaleMatrix = glm::scale(glm::mat4(), scalingVector);

	wvpMatrixStruct.projMatrix = glm::perspective(45.0f, ratio, 1.0f, 300.0f);
	wvpMatrixStruct.modelMatrix = glm::translate(glm::mat4(), treePos);
	wvpMatrixStruct.viewMatrix = glm::lookAt(	
		camPos,
		treePos+targetOffset,
		glm::vec3(0.0f, 1.0f, 0.0f)
		);


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

	glm::mat4x4 world = wvpMatrixStruct.modelMatrix * scaleMatrix;// wvpMatrixStruct.modelMatrix * rotMatrix * scaleMatrix;
	world = glm::rotate(world, (glm::mediump_float)rotation*100.0f, glm::vec3(0.0f, 1.0f, 0.0f));

	glBindBuffer(GL_UNIFORM_BUFFER, matrixUBO);
	glBufferSubData(GL_UNIFORM_BUFFER, 0,						sizeof(glm::mat4),		glm::value_ptr(world));
	glBufferSubData(GL_UNIFORM_BUFFER, sizeof(glm::mat4),		sizeof(glm::mat4),		glm::value_ptr(wvpMatrixStruct.viewMatrix));
	glBufferSubData(GL_UNIFORM_BUFFER, sizeof(glm::mat4)*2,		sizeof(glm::mat4),		glm::value_ptr(wvpMatrixStruct.projMatrix));
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

void GameRenderer::Render(Scene* scene)
{
	//I honestly don't fully know which are needed right now, I add and remove different flags to try and solve problems sometime
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glDisable(GL_CULL_FACE);

	//This is where we render.
	for(unsigned int i = 0; i < renderPasses.size(); ++i)
	{
		renderPasses[i](scene);
	}

	glDisable(GL_BLEND);
	glDisable(GL_DEPTH_TEST);
}

void GameRenderer::SetupRenderPasses()
{
	//So this might look weird. We store each render pass as a std::function
	std::function<void(Scene*)> objRenderPass
		(
		//Copy construct from a lambda
		[&](Scene* scene)
	{
		objShader->Enable();
		objShader->BindBuffers(matrixUBO, 0);

		//Temporary containers...
		Texture currentTexture;
		MaterialStruct currentMaterial;
		FWHandle currentTextureKey = -1;
		FWHandle currentMaterialKey = -1;
		auto& renderables = scene->renderables;

		//For each mesh...
		for(unsigned int renderableIndex = 0; renderableIndex < renderables.size(); ++renderableIndex) 
		{
			auto& mesh = meshHandler->GetMesh(renderables[renderableIndex].meshHandle);
			auto& meshes = mesh.GetSubmeshes();

			/*
			! Update transform based on each model instance !?
			*/

			glBindVertexArray(mesh.vao);

			//For each submesh...
			for(unsigned int submeshIndex = 0; submeshIndex < meshes.size(); ++submeshIndex)
			{
				//Get texkey for this submesh
				FWHandle texKey = meshes[submeshIndex].textureIndex;

				//If it's a different key than the one we have, we change
				if(currentTextureKey != texKey)
				{
					currentTextureKey = texKey;

					if(textureHandler->GetTexture(currentTextureKey, currentTexture))
					{
						objShader->BindTextureSlot(currentTexture.GetTextureID());
					}
				}

				glDrawElementsBaseVertex
					(
					GL_TRIANGLE_STRIP, 
					meshes[submeshIndex].numIndices, 
					GL_UNSIGNED_SHORT,
					(void*)(sizeof(unsigned int) * meshes[submeshIndex].baseIndex), 
					meshes[submeshIndex].baseVertex
					);
				
				//Unbind slot to reset whatever texture was bound
				glBindTexture(GL_TEXTURE_2D, 0);	
			}

			glBindVertexArray(0);
		}

		objShader->ResetState();
		objShader->Disable();
	}
	);


	//Then we insert each render pass in the same order that they will be executed
	renderPasses.push_back(objRenderPass);
}