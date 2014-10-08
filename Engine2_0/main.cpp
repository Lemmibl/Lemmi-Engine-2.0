#pragma once
#include <iostream>
#include "Core systems/EngineCore.h"

#include <easylogging++.h>
_INITIALIZE_EASYLOGGINGPP

int main(int argc, char* argv[])
{
	EngineCore engine;

	//If initialization succeeds
	if(engine.InitializeSystems())
	{
		//Enter main loop. This will run until we quit, where Run() will return either a true or false.
		if(engine.Run())
		{
			//If we've reached this point, engine has finished running and exited with an "Everything has shut down correctly"
		}
		else
		{
			//If we're reached this point, engine has finished running through either a crash or some form of catastrophic failure that it was unable to recover from.

			//First we call shutdown to get rid of the window that is in the way
			engine.Shutdown();

			//And, because we actually crashed with a false, we want to get the info from the error log console that usually prints something interesting
			#ifdef DEBUG
				std::cin.ignore().get();
			#endif
		}
	}

	return 0;
}