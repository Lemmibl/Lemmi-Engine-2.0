#include "MaterialHandler.h"

MaterialHandler::MaterialHandler()
	: HandlerBaseClass(64)
{
	MaterialStruct defaultMaterial;
	defaultMaterial.ambient		= glm::vec3(0.5f, 0.5f, 0.5f);
	defaultMaterial.diffuse		= glm::vec3(1.0f, 1.0f, 1.0f);
	defaultMaterial.specular	= glm::vec3(0.2f, 0.2f, 0.2f);
	defaultMaterial.roughness = 8.0f;

	FWHandle handle;
	StoreMaterial(defaultMaterial, "DefaultMaterial", handle);
}

MaterialHandler::~MaterialHandler()
{
}

bool MaterialHandler::LoadMaterial(aiMaterial* material, std::string filename, FWHandle& outHandle)
{
	if(LookForDuplicateObject(filename, outHandle))
	{
		return true;
	}

	unsigned short index;

	if(objectContainer.AddNewObject(index))
	{
		//Get reference to the material we just created, so that we can write to it.
		auto& newMaterial = objectContainer[index];

		glm::vec3 tempVector(0.8f, 0.8f, 0.8f);

		aiColor4D diffuse;
		if(AI_SUCCESS == aiGetMaterialColor(material, AI_MATKEY_COLOR_DIFFUSE, &diffuse))
		{
			Color4ToFloat3(diffuse, tempVector);
		}

		newMaterial.material.diffuse = tempVector;


		tempVector = glm::vec3(0.2f, 0.2f, 0.2f);
		aiColor4D ambient;
		if(AI_SUCCESS == aiGetMaterialColor(material, AI_MATKEY_COLOR_AMBIENT, &ambient))
		{
			Color4ToFloat3(ambient, tempVector);
		}

		newMaterial.material.ambient = tempVector;

		tempVector =  glm::vec3(0.0f, 0.0f, 0.0f);
		aiColor4D specular;
		if(AI_SUCCESS == aiGetMaterialColor(material, AI_MATKEY_COLOR_SPECULAR, &specular))
		{
			Color4ToFloat3(specular, tempVector);
		}

		newMaterial.material.specular = tempVector;


		newMaterial.material.roughness = 8.0f;

		float shininess;
		unsigned int max;
		if(AI_SUCCESS == aiGetMaterialFloatArray(material, AI_MATKEY_SHININESS, &shininess, &max))
		{
			newMaterial.material.roughness = shininess;
		}

		glGenBuffers(1,					&(newMaterial.uniformBlockIndex));
		glBindBuffer(GL_UNIFORM_BUFFER, newMaterial.uniformBlockIndex);
		glBufferData(GL_UNIFORM_BUFFER, sizeof(newMaterial.material), (void *)(&newMaterial.material), GL_STATIC_DRAW);

		outHandle = CreateHandle(HandleTypes::Material, index);

		InsertNewPair(filename, outHandle);

		return true;
	}

	return false;
}

bool MaterialHandler::StoreMaterial( MaterialStruct material, std::string materialName, FWHandle& outHandle )
{
	if(LookForDuplicateObject(materialName, outHandle))
	{
		return true;
	}

	unsigned short index;

	if(objectContainer.AddNewObject(index))
	{
		//Get reference to the material we just created, so that we can write to it.
		auto& newMaterial = objectContainer[index];

		//Just insert the values
		newMaterial.material = material;

		//Create handle
		outHandle = CreateHandle(HandleTypes::Material, index);

		//Save handle and name pair
		InsertNewPair(materialName, outHandle);

		return true;
	}

	return false;
}

