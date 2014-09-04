#pragma once
#include "ScreenBaseClass.h"

#include "CEGUI/CEGUI.h"

class MainMenuScreen : public ScreenBaseClass
{
public:
	MainMenuScreen();
	~MainMenuScreen();

	virtual bool Enter();
	virtual void Exit();

	bool Initialize();

	virtual bool Update(double deltaTime);
	virtual bool Render(double deltaTime);

private:
	CEGUI::Window* rootWindow;
};

