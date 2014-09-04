#pragma once
#include "SettingsManager.h"
#include <libconfig.h++>

//Okay, so this class is supposed to be inherited from if a class needs to load any settings from a settings file.
//The inheriting class needs to overload OnSettingsReload, wherein it should read the needed settings.
class SettingsDependent
{
	public:
	virtual ~SettingsDependent(){};

	template<class T>
	void InitializeSettings(T* const classPointer)
	{
		//Get settings manager singleton instance.
		SettingsManager& settingsManager = SettingsManager::GetInstance();

		//Class T needs to overload the OnSettingsReload function below.
		settingsManager.GetEvent()->Add((*classPointer), (&T::OnSettingsReload));

		//Calls classPointer->OnSettingsReload. Means that when we call InitializeSettings, settings are also loaded in.
		OnSettingsReload(settingsManager.GetConfig());
	};

	//This function will be called when settingsmanager's event triggers.
	virtual void OnSettingsReload(Config* cfg) = 0;
};

/*
########### Here's an example of how you use SettingsDependent. This example stars my terrain manager from another project.
########### First in the header, you inherit from SettingsDependent:

class TerrainManager : public SettingsDependent

########### Then in the source file, you call SettingsDependent in the constructor:

TerrainManager::TerrainManager() 
: SettingsDependent()
{
	//And inside the constructor, or later in some initialize function, you send the class' 'this' pointer.
	InitializeSettings(this);
}

########### Then when the settings file is loaded or reloaded, your OnSettingsReload function will automatically be called,
########### and you can then load and use your settings.

void TerrainManager::OnSettingsReload(Config* cfg)
{
int type;

const Setting& settings = cfg->getRoot()["terrain"];
settings.lookupValue("startingTerrainType", type);

terrainType = (TerrainTypes::Type)type;
}
*/