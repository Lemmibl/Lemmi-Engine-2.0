#pragma once

//Include some things that are needed for all handler classes
#include <string>
#include <vector>
#include "../../Core systems/Data classes/DODArray.h"
#include "../../Core systems/Data classes/FlyweightHandle.h"

//For FlyweightHandle definitions + functions
using namespace FlyweightHandleFunctions;


template<typename ContainerObjectType, typename ContainerKeyType>
class HandlerBaseClass
{
public:
	HandlerBaseClass(ContainerKeyType containerSize)
	: objectContainer(containerSize)
	{

	}

	virtual ~HandlerBaseClass()
	{
		filenameAndObjectPairings.clear();
	}

protected:
	bool LookForDuplicateObject(std::string fileName, FlyweightHandle& outHandle)
	{
		//For each mesh that we've stored in our DODArray
		for(unsigned int i = 0; i < filenameAndObjectPairings.size(); ++i)
		{
			//If the filename matches  any of our stored filenames...
			if(filenameAndObjectPairings[i].first == fileName)
			{
				//We return the handle to that object
				outHandle = filenameAndObjectPairings[i].second;

				//And return true to indicate that we've succeeded
				return true;
			}
		}

		//We return false if we haven't saved this filename before
		return false;
	}

	void InsertNewPair(std::string filename, FlyweightHandle handle)
	{
		filenameAndObjectPairings.push_back(std::make_pair<std::string, FlyweightHandle>(filename, handle));
	}

protected:
	DODContainer<ContainerObjectType, ContainerKeyType> objectContainer;
	std::vector<std::pair<std::string, FlyweightHandle>> filenameAndObjectPairings;
};