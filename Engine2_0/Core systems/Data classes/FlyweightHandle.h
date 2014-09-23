#pragma once
#include <xutility>
#include "HandleTypes.h"

namespace FlyweightHandleFunctions
{
	typedef unsigned int FlyweightHandle;

	inline FlyweightHandle CreateHandle(HandleTypes::Type type, unsigned int key)
	{
		FlyweightHandle newHandle;

		//Write type to the first 8 bits.
		//Shift key value to 8 bits into the key. Leaving the first 8 bits free for type storage				
		newHandle = ((type & 0xFF) + (key << 8));

		return std::move(newHandle);
	}

	inline HandleTypes::Type GetType(FlyweightHandle handle)
	{
		return std::move(HandleTypes::Type(handle & 0xFF));
	}

	inline unsigned int GetKey(FlyweightHandle handle)
	{
		return (handle >> 8);
	}
};


