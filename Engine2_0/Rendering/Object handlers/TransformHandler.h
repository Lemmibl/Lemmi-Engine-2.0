#pragma once
#include <glew.h>
#include <GLFW/glfw3.h>
#include "handlerbaseclass.h"
#include "../../Game/EntityTransform.h"

class TransformHandler : public HandlerBaseClass<EntityTransform, unsigned short>
{
public:
	TransformHandler();
	~TransformHandler();

	void Update(double deltaTime);
	FWHandle CreateTransform(glm::vec3 position, 
		glm::vec3 rotation = glm::vec3(0.0f, 1.0f, 0.0f), 
		glm::vec3 scale = glm::vec3(1.0f),
		glm::vec3 velocity = glm::vec3(0.0f, 0.0f, 0.0f));

	EntityTransform& GetTransform(FWHandle handle)
	{
		return objectContainer[GetKey(handle)];
	}

private:
	double rotationalSpeed;
	GLFWwindow* currentContext;
};

