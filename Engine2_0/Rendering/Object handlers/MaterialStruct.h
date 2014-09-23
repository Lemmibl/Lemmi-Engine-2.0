#pragma once
#include <Dense>

using namespace Eigen;

struct MaterialStruct
{
	Vector3f specular; //Scale the specular color and strength
	Vector3f diffuse; //Scale diffuse light color and strength
	Vector3f ambient; //You know the drill
	float roughness; //How "big" the specular area is
	bool transparent;
};