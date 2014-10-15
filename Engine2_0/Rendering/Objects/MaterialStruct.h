#pragma once
#include <glew.h>
#include <GLFW/glfw3.h>
#include <glm/vec3.hpp>

//I use vec4 because they're going to be forced to 16byte chunks anyway
struct MaterialStruct
{
	glm::vec3 diffuse; //Scale the specular color and strength
	glm::vec3 ambient; //Scale diffuse light color and strength
	glm::vec3 specular; //You know the drill
	float roughness; //How "big" the specular area is
};

struct MaterialKeyPairing
{
	MaterialStruct material;
	GLuint uniformBlockIndex;
};