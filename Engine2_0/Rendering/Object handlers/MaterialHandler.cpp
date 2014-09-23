#include "MaterialHandler.h"


MaterialHandler::MaterialHandler()
: materialContainer(64)
{
}

MaterialHandler::~MaterialHandler()
{
}

HandleFunctions::FlyweightHandle MaterialHandler::LoadMaterial( std::string filename, aiMaterial* material )
{

}

bool MaterialHandler::LookForDuplicateMaterial(std::string fileName, FlyweightHandle& outHandle)
{
	//For each mesh that we've stored in our DODArray
	for(unsigned int i = 0; i < fileNameAndMaterialPairings.size(); ++i)
	{
		//If the filename matches  any of our stored filenames...
		if(fileNameAndMaterialPairings[i].first == fileName)
		{
			//We return the handle to that object
			outHandle = fileNameAndMaterialPairings[i].second;

			//And return true to indicate that we've succeeded
			return true;
		}
	}

	//We return false if we haven't saved this filename before
	return false;
}

void MaterialHandler::InsertNewPair(std::string filepath, FlyweightHandle handle)
{
	fileNameAndMaterialPairings.push_back(std::make_pair<std::string, FlyweightHandle>(filepath, handle));
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