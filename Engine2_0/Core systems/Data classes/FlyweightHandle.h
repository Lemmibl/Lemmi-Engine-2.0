#pragma once
#include <xutility>
#include "HandleTypes.h"

namespace HandleFunctions
{
	struct FlyweightHandle
	{
		//Inside this key, the first 8 bits are HandleType and the other 24 bits are the actual value bits
		//2^8 == 256. 2^24 == 16777216 16 mil should be enough keys
		unsigned int key;
	};

	FlyweightHandle CreateHandle(HandleTypes::Type type, unsigned int key);
	HandleTypes::Type GetType(const FlyweightHandle& handle);
	unsigned int GetKey(const FlyweightHandle& handle);
};


