#pragma once

//Include some things that are needed for all handler classes
#include "../../Core systems/Data classes/DODArray.h"
#include "../../Core systems/Data classes/FWHandle.h"

#include <string>
#include <vector>

//For FlyweightHandle definitions + functions
using namespace FlyweightFunctionality;


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
	bool LookForDuplicateObject(std::string fileName, FWHandle& outHandle)
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

	void InsertNewPair(std::string filename, FWHandle handle)
	{
		filenameAndObjectPairings.push_back(std::make_pair<std::string, FWHandle>(filename, handle));
	}

protected:
	DODContainer<ContainerObjectType, ContainerKeyType> objectContainer;
	std::vector<std::pair<std::string, FWHandle>> filenameAndObjectPairings;
};