/*
http://stackoverflow.com/questions/86582/singleton-how-should-it-be-used
http://stackoverflow.com/questions/10892167/proper-implementation-of-global-configuration

* C++ Singleton
* Limitation: Single Threaded Design
* See: http://www.aristeia.com/Papers/DDJ_Jul_Aug_2004_revised.pdf
*      For problems associated with locking in multi threaded applications
*
* Limitation:
* If you use this Singleton (A) within a destructor of another Singleton (B)
* This Singleton (A) must be fully constructed before the constructor of (B)
* is called.
*/

#pragma once
#include "../Events/SchladetschEvents.h"
#include <iomanip>
#include <cstdlib>
#include <libconfig.h++>

using namespace Schladetsch::Events;
using namespace libconfig;

//K, so everything else in this project is a CEGUI string, but this string won't at all be used by the gui library, it's just going to be sent into the libconfig library. Hence std::string.
static const std::string filepath = "../TCPChatServer/Settings/settings.cfg";

class SettingsManager
{
private:
	// Private Constructor
	SettingsManager()
	{
		//Load config file upon construction
		cfg.readFile(filepath.c_str());
	}

	//Stop the compiler from generating ways to copy the object
	SettingsManager(SettingsManager const& copy);            // Not Implemented
	SettingsManager& operator=(SettingsManager const& copy); // Not Implemented

public:
	//The singleton function
	static SettingsManager& GetInstance()
	{
		// The only instance
		// Guaranteed to be lazily initialized
		// Guaranteed that it will be destroyed correctly when the program is ended.
		static SettingsManager instance;

		return instance;
	}

	//As separate function because we also want to trigger the reload event
	void LoadSettings()
	{
		//Load file
		cfg.readFile(filepath.c_str());

		//Shoot off event that we should reload all values that come from the settings file in case anything has been updated.
		loadEvent(&cfg);
	} 

	Config* GetConfig() { return &cfg; }

	/*
	Used like this, to add Foo::Method to the event call list:

	Foo foo;
	event.Add(foo, &Foo::Method);
	*/
	Event<Config*>* GetEvent() { return &loadEvent; }

private:
	//This will be the event that we trigger every time we load the settings file.
	//Other classes will subscribe to the event, so that they can re-read the settings in case anything has changed.
	Event<Config*> loadEvent;

	//libconfig class that probably contains all the data that gets loaded in from the .cfg settings file.
	Config cfg;
};