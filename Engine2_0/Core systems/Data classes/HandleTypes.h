#pragma once

namespace HandleTypes
{
	static const unsigned int InvalidHandle = 0;

	//Will be the highest value possible... cuz unsigned int wraparound
	typedef unsigned int Type;

	//Don't go above 256 here. We want to stay within the first 8 bits
	enum
	{
		Mesh = 1,
		Texture = 2,
		Material = 4,
		Model = 8,
		Renderable = 16,
		Collidable = 32,
		AI = 64,
		Transform = 128
		//TODO: more types...
	};
}