#pragma once
#include <glew.h>
#include <GLFW/glfw3.h>
#include <string>
#include <vector>
#include "../../Core systems/Data classes/DODArray.h"
#include "../../Core systems/Data classes/FlyweightHandle.h"

#include <assimp/scene.h>

using namespace HandleFunctions;

class TextureHandler
{
public:
	TextureHandler();
	~TextureHandler();

	GLuint GetTexture(FlyweightHandle handle);

	FlyweightHandle LoadTexture(std::string filename, aiMaterial* material);

private:
	bool LookForDuplicateTexture(std::string fileName, FlyweightHandle& outHandle);
	void InsertNewPair(std::string filename, FlyweightHandle handle);


private:
	DODContainer<GLuint, unsigned short> textureContainer;
	std::vector<std::pair<std::string, FlyweightHandle>> fileNameAndTexturePairings;
};

