#pragma once

namespace HandleTypes
{
	typedef unsigned int Type;

	//Don't go above 256 here. We want to stay within the first 8 bits
	enum
	{
		Mesh = 0,
		Texture,
		Material,
		Model,
		Renderable,
		Collidable,
		AI
		//TODO: more types...
	};
}