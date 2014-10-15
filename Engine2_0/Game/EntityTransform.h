#pragma once
#include <glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/mat4x4.hpp>

//Struct to represent an entity in the world
struct EntityTransform
{
	glm::mat4x4 worldMatrix;
	//glm::quat orientation;
	glm::vec3 rotation; //yaw pitch roll?
	glm::vec3 position;
	glm::vec3 velocity; //Direction it might be going in
	glm::vec3 scale; //Currently just care about uniform scale
	//float mass;
};

//struct TransformKeyPair
//{
//	EntityTransform transform;
//	GLuint bufferIndex; //Where openGL stores this uniform buffer
//};