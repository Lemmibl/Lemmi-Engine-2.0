#pragma once

#include <glew.h>
#include <GLFW/glfw3.h>
#include <assimp/scene.h>

#include "../Objects/MaterialStruct.h"
#include "HandlerBaseClass.h"

#include <glm/vec3.hpp>
#include <glm/vec4.hpp>

class MaterialHandler : HandlerBaseClass<MaterialKeyPairing, unsigned short>
{
public:
	MaterialHandler();
	~MaterialHandler();

	bool StoreMaterial(MaterialStruct material, std::string materialName, FWHandle& outHandle);
	bool LoadMaterial(aiMaterial* material, std::string filename, FWHandle& outHandle);

	bool GetMaterial(FWHandle handle, MaterialKeyPairing& outMaterial) 
	{
		if(objectContainer.IsValid(handle))
		{
			outMaterial = objectContainer[handle];
			return true;
		}

		return false;
	};

private:
	inline void Color4ToFloat4(aiColor4D& aiColor, glm::vec4& float4)
	{
		float4.x = aiColor.r;
		float4.y = aiColor.g;
		float4.z = aiColor.b;
		float4.w = aiColor.a;
	}

	inline void Color4ToFloat3(aiColor4D& aiColor, glm::vec3& float3)
	{
		float3.x = aiColor.r;
		float3.y = aiColor.g;
		float3.z = aiColor.b;
	}
};

