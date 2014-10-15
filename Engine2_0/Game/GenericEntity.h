#pragma once
#include <unordered_map>
#include "../Core systems/Data classes/FWHandle.h"
#include "../Core systems/Data classes/HandleTypes.h"

using namespace FlyweightFunctionality;
using namespace HandleTypes;

class GenericEntity
{
private:
	//Denotes what sort of objects this entity contains.
	unsigned int flags; //So for example it can be: flags = Renderable|Transform|Physics|AI and it'd contain an array of four objects

	//Container for our objects
	std::unordered_map<HandleTypes::Type, FWHandle> keys;

public:
	GenericEntity()
	: flags(0)
	{
	}

	~GenericEntity()
	{
	}
	
	FWHandle& operator[](const HandleTypes::Type t)				
	{ 
		return keys[t];
	}

	bool ContainsComponent(Type type)
	{
		//Look up the corresponding value in our flags object
		return (flags & type) ? true : false;
	}

	void AddComponent(HandleTypes::Type type, FlyweightFunctionality::FWHandle key)
	{
		//If we don't already have this component
		if(!ContainsComponent(type))
		{
			keys.insert(std::make_pair(type, key));

			//Record that we contain this type
			flags |= type;
		}
	}

	void EditComponent(HandleTypes::Type type, FlyweightFunctionality::FWHandle key)
	{
		if(ContainsComponent(type))
		{
			//Update value
			keys[type] = key;
		}
	}

	FWHandle GetComponent(HandleTypes::Type type)
	{
		//If we have this component
		if(ContainsComponent(type))
		{
			return keys[type];
		}

		return HandleTypes::InvalidHandle;
	}
};