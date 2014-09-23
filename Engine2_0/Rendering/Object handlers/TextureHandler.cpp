#include "TextureHandler.h"

#include <SOIL2.h>

static const std::string baseFilePath = "../Engine2_0/Data/Textures/";

TextureHandler::TextureHandler()
: textureContainer(64)
{
}


TextureHandler::~TextureHandler()
{
}

HandleFunctions::FlyweightHandle TextureHandler::LoadTexture(std::string filename, aiMaterial* material)
{
	FlyweightHandle newHandle;

	if(LookForDuplicateTexture(filename, newHandle))
	{
		return newHandle;
	}

	unsigned short newTextureHandle;

	if(textureContainer.AddNewObject(newTextureHandle))
	{
		textureContainer[newTextureHandle] = NULL;

		//If the material contains any diffuse textures
		if(material->GetTextureCount(aiTextureType_DIFFUSE) > 0)
		{
			aiString textureName;

			//See if we can fetch a diffuse texture from the material
			if(material->GetTexture(aiTextureType_DIFFUSE, 0, &textureName, NULL, NULL, NULL, NULL, NULL) == AI_SUCCESS) 
			{
				std::string fullPath = baseFilePath + textureName.C_Str();

				//Load texture and automatically store it inside openGL's ... innards.
				GLuint oglTexHandle = SOIL_load_OGL_texture
				(
					fullPath.c_str(),
					SOIL_LOAD_AUTO,
					SOIL_CREATE_NEW_ID,
					0 //No flags right now
				);

				//Save it in our container.
				textureContainer[newTextureHandle] = oglTexHandle;

				//Make sure we encode the handle properly
				newHandle = CreateHandle(HandleTypes::Texture, newTextureHandle);
			}
		}
	}

	return newHandle;
}


bool TextureHandler::LookForDuplicateTexture( std::string fileName, FlyweightHandle& outHandle )
{
	//For each mesh that we've stored in our DODArray
	for(unsigned int i = 0; i < fileNameAndTexturePairings.size(); ++i)
	{
		//If the filename matches  any of our stored filenames...
		if(fileNameAndTexturePairings[i].first == fileName)
		{
			//We return the handle to that object
			outHandle = fileNameAndTexturePairings[i].second;

			//And return true to indicate that we've succeeded
			return true;
		}
	}

	//We return false if we haven't saved this filename before
	return false;
}

void TextureHandler::InsertNewPair(std::string filepath, FlyweightHandle handle)
{
	fileNameAndTexturePairings.push_back(std::make_pair<std::string, FlyweightHandle>(filepath, handle));
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