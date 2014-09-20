#pragma once

namespace HandleTypes
{
	typedef unsigned int Type;

	//Don't go above 256 here. We want to stay within the first 8 bits
	enum
	{
		Renderable = 0,
		Collidable,
		AI
		//TODO: more types...
	};
}