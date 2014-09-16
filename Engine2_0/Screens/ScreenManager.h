#pragma once
#include <memory>
#include <unordered_map>
#include "ScreenStates.h"

class ScreenBaseClass;

class ScreenManager
{
public:
	ScreenManager();
	~ScreenManager();

	bool Initialize();

	bool Update(double deltaTime);
	void Render(double deltaTime);
	void SwitchState(ScreenStates::State newState);
	bool IsRunning() { return running; }

private:
	void AddNewState(ScreenStates::State screenEnum, std::unique_ptr<ScreenBaseClass> screen);

private:
	bool running;
	double glfwTime;

	//This is where we keep all the ptrs to the different screen states
	std::unordered_map<ScreenStates::State, std::unique_ptr<ScreenBaseClass>> screenLookupTable;

	//Using raw pointer because it's not supposed to own anything, and thus not be in charge of deleting anything either, as a smart ptr would.
	ScreenBaseClass* currentScreen;
};