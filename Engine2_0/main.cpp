#pragma once
//#include "Libraries/VisualLeakDetector/include/vld.h"
#include "Core systems/EngineCore.h"

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
		}
	}


	return 0;
}