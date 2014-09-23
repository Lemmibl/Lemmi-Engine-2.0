#include "Model.h"

Model::Model()
{
}

Model::~Model()
{
	meshHandle = CreateHandle(HandleTypes::Model, 0);
	textureHandles.clear();
	materialHandles.clear();
}