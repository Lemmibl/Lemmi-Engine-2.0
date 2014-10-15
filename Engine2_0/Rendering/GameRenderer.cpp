#include "GameRenderer.h"

#include "Object handlers/TextureHandler.h"
#include "Object handlers/MaterialHandler.h"
#include "Object handlers/MeshHandler.h"
#include "Object handlers/TransformHandler.h"
#include "Objects/Texture.h"

#include <easylogging++.h>

GameRenderer::GameRenderer()
{
	rotation = 0.0f;
}


GameRenderer::~GameRenderer()
{
}

bool GameRenderer::Initialize(MaterialHandler* mtlHandlerPtr, MeshHandler* meshHandlerPtr, TextureHandler* texHandlerPtr, TransformHandler* transformHandlerPtr)
{
	materialHandler = mtlHandlerPtr;
	meshHandler = meshHandlerPtr;
	textureHandler = texHandlerPtr;
	transformHandler = transformHandlerPtr;

	glfwWindow = glfwGetCurrentContext();

	glfwGetFramebufferSize(glfwWindow, &screenWidth, &screenHeight);

	float ratio = (float)screenWidth / (float) screenHeight;
	glViewport(0, 0, screenWidth, screenHeight);
	glClearColor(0.4f, 0.4f, 0.5f, 1.0f);

	glm::vec3 camPos(0.0f, 25.0f, -70.0f);
	glm::vec3 treePos(0.0f, 0.0f, 0.0f);
	glm::vec3 scalingVector(1.5f);
	glm::vec3 targetOffset = glm::vec3(0.0f, 6.0f, 0.0f) * scalingVector;

	scaleMatrix = glm::scale(glm::mat4(), scalingVector);

	wvpMatrixStruct.projMatrix = glm::perspective(45.0f, ratio, 0.25f, 300.0f);
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
	//if(rotation > 360.0)
	//{
	//	rotation -= 360.0;
	//}

	//rotation += deltaTime;

	//glm::mat4x4 world = wvpMatrixStruct.modelMatrix * scaleMatrix;// wvpMatrixStruct.modelMatrix * rotMatrix * scaleMatrix;
	//world = glm::rotate(world, (glm::mediump_float)rotation*100.0f, glm::vec3(0.0f, 1.0f, 0.0f));

	//glBindBuffer(GL_UNIFORM_BUFFER, matrixUBO);
	//glBufferSubData(GL_UNIFORM_BUFFER, 0,							sizeof(glm::mat4),		glm::value_ptr(world));
	//glBufferSubData(GL_UNIFORM_BUFFER, sizeof(glm::mat4),			sizeof(glm::mat4),		glm::value_ptr(wvpMatrixStruct.viewMatrix));
	//glBufferSubData(GL_UNIFORM_BUFFER, sizeof(glm::mat4)*2,		sizeof(glm::mat4),		glm::value_ptr(wvpMatrixStruct.projMatrix));
	//glBindBuffer(GL_UNIFORM_BUFFER, 0);
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
		objShader->BindVPMatrixBuffer(matrixUBO);

		//Temporary containers...
		Texture currentTexture;
		MaterialStruct currentMaterial;
		FWHandle currentTextureKey = -1;
		FWHandle currentMaterialKey = -1;
		auto& renderables = scene->renderables;

		//For each mesh...
		for(unsigned int renderableIndex = 0; renderableIndex < renderables.size(); ++renderableIndex) 
		{
			//For each renderable, we update the model matrix.
			objShader->BindModelMatrix(transformHandler->GetTransform(renderables[renderableIndex].transformHandle).worldMatrix);

			//Get temporary references to things so that we don't have to call accessors/do lookups every time they're used
			//Also shortening the names for better readability
			auto& mesh = meshHandler->GetMesh(renderables[renderableIndex].meshHandle);
			auto& subMeshes = mesh.GetSubmeshes();

			glBindVertexArray(mesh.vao);

			//For each submesh...
			for(unsigned int submeshIndex = 0; submeshIndex < subMeshes.size(); ++submeshIndex)
			{
				//Get texkey for this submesh
				FWHandle texKey = subMeshes[submeshIndex].textureIndex;

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
					GL_TRIANGLES, 
					subMeshes[submeshIndex].numIndices, 
					GL_UNSIGNED_INT,
					(void*)(sizeof(GLuint)*(subMeshes[submeshIndex].baseIndex)), 
					subMeshes[submeshIndex].baseVertex
					);
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

/*
glDrawElements
(
GL_TRIANGLES,
meshes[submeshIndex].numIndices,
GL_UNSIGNED_SHORT,
(void*)(meshes[submeshIndex].baseIndex)
);
*/


/* http://stackoverflow.com/questions/10289555/rendering-a-mesh-in-opengl-as-a-series-of-subgroups
Yes, you should draw each sub-group separately from the others. This is required till the state is different between sub-groups.

But you are making a too long step.

To avoid multiple draw calls, you can introduce a vertex attribute indicating an index used for accessing 
uniform array values (array of materials, array of textures). In this way, you need only one draw call, 
but you will have the cost of one additional attribute and its relative management.

I would avoid the above approach. What if a sub-group is textured and another one not? 
How do you discriminate whether to texture or not? Introducing other attributes? Seems confusing.

The first point is that the buffer object management is very flexible. 
Indeed you could have a single element buffer object and a single vertex buffer object: using offsets and interleaving 
you can satisfy every level of complexity. And then, on modern harware, using vertex 
array objects you can minimize the cost of the different buffer bindings.

Second point is that your software can group different sub-group having the same uniform state, 
joining multiple draw calls into a single one. Remember that you can use Multi draw entry points variants, 
and there's also the primitive restart that can aid you (in the case stripped primitives).

Other considerations are not usefull, because you have to draw anyway, regardless if it's complex or not. 
Successively, when you have a correct rendering, you could profile the application and the rendering, cutting-off the hot-spots.
*/