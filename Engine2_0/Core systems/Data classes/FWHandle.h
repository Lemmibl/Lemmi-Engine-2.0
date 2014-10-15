#pragma once
#include <xutility>
#include "HandleTypes.h"

namespace FlyweightFunctionality
{
	typedef unsigned int FWHandle;

	inline FWHandle CreateHandle(HandleTypes::Type type, unsigned int key)
	{
		FWHandle newHandle;

		//Write type to the first 8 bits.
		//Shift key value to 8 bits into the key. Leaving the first 8 bits free for type storage				
		newHandle = ((type & 0xFF) + (key << 8));

		return std::move(newHandle);
	}

	inline HandleTypes::Type GetType(FWHandle handle)
	{
		return (handle & 0xFF);
	}

	inline unsigned int GetKey(FWHandle handle)
	{
		return (handle >> 8);
	}
};


