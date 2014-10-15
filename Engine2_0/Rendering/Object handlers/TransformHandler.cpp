#include "TransformHandler.h"

// glm::translate, glm::rotate, glm::scale, glm::perspective
#include <glm/gtc/matrix_transform.hpp>

TransformHandler::TransformHandler()
	: HandlerBaseClass(128)
{
	rotationalSpeed = 40.0;
	currentContext = glfwGetCurrentContext();
}


TransformHandler::~TransformHandler()
{
}

void TransformHandler::Update(double deltaTime)
{
	if(glfwGetKey(currentContext, GLFW_KEY_R) == GLFW_PRESS)
	{
		auto objects = objectContainer.GetObjectArray();

		for(unsigned int index = 0; index < objectContainer.GetActiveObjectCount(); ++index)
		{
			objectContainer[index].worldMatrix = glm::rotate(objectContainer[index].worldMatrix, (glm::mediump_float)(rotationalSpeed * deltaTime), objectContainer[index].rotation);
		}
	}
}

FWHandle TransformHandler::CreateTransform(	glm::vec3 position, 
	glm::vec3 rotation, 
	glm::vec3 scale,
	glm::vec3 velocity
	)
{
	//When we create a transform, we don't look for duplicates unlike other handlers. There are no duplicate transforms, they're all supposed to be unique.
	//If you want two objects to use the same transform, just copy the handle manually.

	FWHandle outHandle = HandleTypes::InvalidHandle;

	unsigned short objectHandle;

	if(objectContainer.AddNewObject(objectHandle))
	{
		//Get the reference to the object
		EntityTransform& newTransform = objectContainer[objectHandle];

		//Fill up the object
		newTransform.position	= position;
		newTransform.rotation	= rotation;
		newTransform.scale		= scale;
		newTransform.velocity	= velocity;

		newTransform.worldMatrix = glm::translate(glm::mat4(), position);
		newTransform.worldMatrix = glm::scale(newTransform.worldMatrix, scale);
		newTransform.worldMatrix = glm::rotate(newTransform.worldMatrix, 0.0f, rotation);

		outHandle = CreateHandle(HandleTypes::Transform, objectHandle);
	}

	return outHandle;
}
