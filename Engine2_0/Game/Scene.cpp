#include "Scene.h"

#include <easylogging++.h>
#include "../Rendering/Object handlers/ModelHandler.h"

Scene::Scene()
{
}

Scene::~Scene()
{
}

void Scene::Load(ModelHandler* modelHandlerPtr)
{
	modelHandler = modelHandlerPtr;

	std::string eucalyptus = "eucalyptus.obj";
	std::string sphere = "sphere.obj";
	std::string pine = "pineish.obj";

	std::string activeModel = eucalyptus;

	for(unsigned int i = 0; i < 10; ++i)
	{
		FWHandle modelHandle;

		if(!modelHandler->LoadModel(activeModel, modelHandle))
		{
			LOG(ERROR) << ("Couldn't load file " + activeModel + " in modelHandler!");
		}
		else
		{
			renderables.push_back(modelHandler->GetModel(modelHandle));
		}
	}

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
	glBindVertexArray(vertexAttributeObject);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObject);
	glVertexAttribPointer (0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
}

void Scene::Update( double deltaTime )
{
	for(unsigned int i = 0; i < renderables.size(); ++i)
	{
		/*
		Possibly do culling first
		*/

		/*
		Sort all submeshes? Renderables?
		*/
	}
}
