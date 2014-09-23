#include "MaterialHandler.h"


MaterialHandler::MaterialHandler()
: HandlerBaseClass(64)
{
}

MaterialHandler::~MaterialHandler()
{
}

FlyweightHandleFunctions::FlyweightHandle MaterialHandler::LoadMaterial( std::string filename, aiMaterial* material )
{
	FlyweightHandle returnHandle;

	if(LookForDuplicateObject(filename, returnHandle))
	{
		return returnHandle;
	}



	InsertNewPair(filename, returnHandle);

	return returnHandle;
}

/*
bool Mesh::InitMaterials(const aiScene* pScene, const std::string& Filename)
{
// Extract the directory part from the file name
std::string::size_type SlashIndex = Filename.find_last_of("/");
std::string Dir;

if (SlashIndex == std::string::npos) {
Dir = ".";
}
else if (SlashIndex == 0) {
Dir = "/";
}
else {
Dir = Filename.substr(0, SlashIndex);
}

bool Ret = true;

// Initialize the materials
for (unsigned int i = 0 ; i < pScene->mNumMaterials ; i++) {
const aiMaterial* pMaterial = pScene->mMaterials[i];

m_Textures[i] = NULL;

if (pMaterial->GetTextureCount(aiTextureType_DIFFUSE) > 0) {
aiString Path;

if (pMaterial->GetTexture(aiTextureType_DIFFUSE, 0, &Path, NULL, NULL, NULL, NULL, NULL) == AI_SUCCESS) {
std::string FullPath = Dir + "/" + Path.data;
m_Textures[i] = new Texture(GL_TEXTURE_2D, FullPath.c_str());

if (!m_Textures[i]->Load()) {
printf("Error loading texture '%s'\n", FullPath.c_str());
delete m_Textures[i];
m_Textures[i] = NULL;
Ret = false;
}
else {
printf("Loaded texture '%s'\n", FullPath.c_str());
}
}
}

// Load a white texture in case the model does not include its own texture
if (!m_Textures[i]) {
m_Textures[i] = new Texture(GL_TEXTURE_2D, "./white.png");

Ret = m_Textures[i]->Load();
}
}

return Ret;
}

*/