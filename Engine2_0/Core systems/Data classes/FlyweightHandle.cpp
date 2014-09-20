#include "FlyweightHandle.h"

namespace HandleFunctions
{
	FlyweightHandle CreateHandle(HandleTypes::Type type, unsigned int key)
	{
		FlyweightHandle newHandle;

		//Write type to the first 8 bits.
		//Shift key value to 8 bits into the key. Leaving the first 8 bits free for type storage				
		newHandle.key = ((type & 0xFF) + (key << 8));

		return std::move(newHandle);
	}

	HandleTypes::Type GetType(const FlyweightHandle& handle)
	{
		return std::move(HandleTypes::Type(handle.key & 0xFF));
	}

	unsigned int GetKey(const FlyweightHandle& handle)
	{
		return (handle.key >> 8);
	}
}