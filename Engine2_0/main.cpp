#pragma once
#include <iostream>
#include "Core systems/EngineCore.h"

#include <easylogging++.h>
_INITIALIZE_EASYLOGGINGPP

int main(int argc, char* argv[])
{
	EngineCore engine;

	//Seed our random...
	srand(time(0));

	//If initialization succeeds
	if(engine.InitializeSystems())
	{
		//Enter main loop. This will run until we quit, where Run() will return either a true or false.
		engine.Run();

		//First we call shutdown to get rid of the window that is in the way
		engine.Shutdown();

		//And, because we might have crashed, we want to get the info from the error log console that usually prints something interesting
		#ifdef DEBUG
			//std::cin.get();
		#endif
	}

	return 0;
}