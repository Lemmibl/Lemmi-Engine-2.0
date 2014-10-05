#pragma once
#include <glew.h>
#include <GLFW/glfw3.h>

#include "CEGUI/CEGUI.h"

#include "../Libraries/Events/SchladetschEvents.h"
#include "ScreenStates.h"

class ScreenBaseClass
{
public:
	ScreenBaseClass();
	virtual ~ScreenBaseClass();

	virtual bool Enter() = 0;
	virtual void Exit() = 0;

	virtual bool Update(double deltaTime) = 0;
	virtual bool Render(double deltaTime) = 0;

	Schladetsch::Events::Event<ScreenStates::State>* GetStateChangeEvent() { return &stateChangeEvent; }
	const bool IsActive() const { return isActive; }
	const bool HasBeenInitialized() const { return isInitialized; }

protected:
	void SetInitialized(bool val) { isInitialized = val; }
	void SetActivated(bool val) { isActive = val; }
	Schladetsch::Events::Event<ScreenStates::State> stateChangeEvent;

private:
	bool isActive;

	//Bool to trigger initialization first time it's entered.
	bool isInitialized;
};