#pragma once
#include <GLFW/glfw3.h>
#include <string>

//Will handle everything CEGUI. Initialization, lifetime, updating.
class CEGUIWrapper
{
public:
	CEGUIWrapper();
	~CEGUIWrapper();

	//Cleanup
	void Shutdown();

	//Set up all subsystems, initialize renderer, load fonts etc
	bool Initialize();

	//Inject stuff like deltaTime into CEGUI
	void Update(double deltaTime);

	//Draw CEGUI elements
	void Render();
};