#include "Scene.h"

#include <easylogging++.h>
#include "../Rendering/Object handlers/ModelHandler.h"
#include "../Rendering/Object handlers/TransformHandler.h"

#include "GenericEntity.h"

Scene::Scene()
{
}

Scene::~Scene()
{
	renderables.clear();
}

void Scene::Load(ModelHandler* modelHandlerPtr, TransformHandler* transformHandlerPtr)
{
	//Need to setup the seed because glm::rand uses C random api
	modelHandler = modelHandlerPtr;
	transformHandler = transformHandlerPtr;

	//modelNames.push_back("sphere.obj");
	modelNames.push_back("Eucalyptus.obj");
	modelNames.push_back("eucalyptus2.obj");
	modelNames.push_back("eucalyptus3.obj");
	modelNames.push_back("pineish.obj");
	modelNames.push_back("bush.obj");

	float x, y, z;
	FWHandle result = 0;

	for(unsigned int i = 0; i < 25; ++i)
	{
		//Generate a random position for the object
		x = RandFloat(-15.0f, 15.0f);
		y = 0.0f;	   
		z = RandFloat(-15.0f, 15.0f);

		glm::vec3 randomPosition = glm::vec3(x, y, z);

		//Randomly select which model to load
		unsigned int randomIndex = static_cast<unsigned int>(glm::round(RandFloat(0.0f, static_cast<float>(modelNames.size()-1))));
		std::string activeModel = modelNames[randomIndex];

		result = modelHandler->CreateModelInstance(activeModel, randomPosition, glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(1.0f));

		if(result == InvalidHandle)
		{
			LOG(ERROR) << ("Couldn't load file " + activeModel + " in modelHandler!");
		}
		else
		{
			//Copy model into renderables vector.
			renderables.push_back(modelHandler->GetModel(result));
		}
	}

	std::sort(renderables.begin(), renderables.end(), MeshHandleSort());
}

void Scene::Update(double deltaTime)
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