#pragma once
#include "ScreenBaseClass.h"

class GameplayScreen : public ScreenBaseClass
{
public:
	GameplayScreen();
	~GameplayScreen();

	virtual bool Enter();
	virtual void Exit();

	virtual bool Update( double deltaTime );
	virtual bool Render( double deltaTime );

};

