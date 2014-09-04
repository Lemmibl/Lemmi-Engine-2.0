#pragma once

namespace ScreenStates
{
	typedef int State;

	enum
	{
		Quit = 0,
		MainMenu,
		InGame,
		OptionsMenu,
		Loading
	};
}