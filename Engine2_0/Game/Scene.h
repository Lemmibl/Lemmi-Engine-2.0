#pragma once
#include <windows.h>
#include <glew.h>
#include <GLFW/glfw3.h>

#include <vector>
#include "Camera.h"
#include "../Core systems/Data classes/FWHandle.h"
#include "../Rendering/Objects/ModelInstance.h"

// glm::vec3, glm::vec4, glm::ivec4, glm::mat4
#include <glm/glm.hpp>

// glm::translate amongst other useful matrix stuff
#include <glm/gtc/matrix_transform.hpp>

#include <glm/gtc/random.hpp>

using namespace FlyweightFunctionality;

class ModelHandler;
class TransformHandler;

class Scene
{
//To access all the private goodies in here without a billion getters/setters.
//It's better encapsulation this way than just setting everything to public.
friend class GameRenderer;

private:
	struct MeshHandleSort
	{
		inline bool operator() (const ModelInstance& modelA, const ModelInstance& modelB)
		{
			return (modelA.meshHandle < modelB.meshHandle);
		}
	};


public:
	Scene();
	~Scene();

	//In the future we'll probably have, like, a string to specify a level name or something along those lines. A random seed, maybe.
	void Load(ModelHandler* modelHandler, TransformHandler* transformHandler);

	void Update(double deltaTime);

private:
	inline float RandFloat(const float min, const float max)
	{
		return glm::linearRand<float>(min, max);
	}

private:
	Camera playerCamera;
	ModelHandler* modelHandler;
	TransformHandler* transformHandler;

	std::vector<ModelInstance> renderables;
	std::vector<std::string> modelNames;
};

