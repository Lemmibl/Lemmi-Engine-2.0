//Credits where credits are due; this class is very much based upon the "ID lookup table" data structure by Niklas Frykholm as Bitsquid:
//http://bitsquid.blogspot.se/2011/09/managing-decoupling-part-4-id-lookup.html

#pragma once
#include <limits>

//So ObjectType is what the container will be holding,
//and IndexType is what type of object we will be using to keep track of everything,
//i.e. unsigned short or an unsigned int etc
template<typename ObjectType, typename IndexType>
class DODContainer
{

	/************************************************************************/
	/*						Various definitions                             */
	/************************************************************************/

private:
	struct ObjectContainer
	{
		ObjectContainer(IndexType size)
		{
			idArray = new unsigned short[size];
			dataArray = new ObjectType[size];
		}

		~ObjectContainer()
		{
			delete[] idArray;
			delete[] dataArray;
		}

		unsigned short* idArray;
		ObjectType* dataArray;
	};

	struct IndexObject
	{
		unsigned short index;
		unsigned short next;
	};

public:

	/************************************************************************/
	/*                    Constructors and destructors                      */
	/************************************************************************/
	DODContainer(IndexType size)
	:	OBJECT_IS_EMPTY(std::numeric_limits<IndexType>::max()),
		objectArray(size)
	{
		maxObjects = size;
		activeObjects = 0;

		indexArray = new IndexObject[maxObjects];

		for(IndexType i = 0; i < maxObjects; ++i)
		{
			indexArray[i].next = i+1; //Currently just pointing to next, but this can/will change.
			indexArray[i].index = OBJECT_IS_EMPTY; //Just a dummy value...
		}

		freelist_dequeue = 0;
		freelist_enqueue = maxObjects;
	}

	~DODContainer()
	{
		delete[] indexArray;
	}

	/************************************************************************/
	/*                              Operators                               */
	/************************************************************************/
	const ObjectType&	operator[](int i)	const		{ return objectArray.dataArray[i]; }
	ObjectType&			operator[](int i)				{ return objectArray.dataArray[i]; }

	/************************************************************************/
	/*                          Other functions                             */
	/************************************************************************/
	IndexType GetActiveObjectCount() 
	{ 
		return activeObjects; 
	}

	bool IsValid(IndexType handle)
	{
		if(indexArray[handle].index != OBJECT_IS_EMPTY)
		{
			return true;
		}

		return false;
	}

	//For iteration!
	ObjectContainer* Begin() { return objectArray.dataArray; }
	ObjectContainer* End() { return objectArray.dataArray+activeObjects; }

	ObjectType& GetSpecificObject(IndexType handle) 
	{ 
		return objectArray.dataArray[indexArray[handle].index]; 
	}

	ObjectType* GetObjectArray()
	{
		return objectArray.dataArray;
	}

	//Returns true if we've succeeded in adding the new object
	bool AddNewObject(IndexType& outHandle)
	{
		if(activeObjects < maxObjects)
		{
			IndexType indexID = freelist_dequeue;

			//Get a reference to the first available object in our index array
			IndexObject& index = indexArray[indexID];
			freelist_dequeue = index.next;

			//Assign the right value to our index
			index.index = activeObjects++;

			objectArray.idArray[index.index] = indexID;
			outHandle = indexID;

			return true;
		}

		return false;
	}

	//Removes an object, moves things around for optimal linearity 
	// and re-adds the used index to the free list
	void RemoveObject(IndexType externalHandle)
	{
		//Get index to the object that we're going to delete
		IndexObject& idx = indexArray[externalHandle];

		//Get a non-const reference to the object
		ObjectType& object = objectArray.dataArray[idx.index];
		IndexType& index = objectArray.idArray[idx.index];

		//And overwrite the object with the currently furthest out object
		//While at the same time reducing activeObjects count with one.
		//Essentially we delete and copy in one execution
		object = objectArray.dataArray[activeObjects];
		index = objectArray.idArray[--activeObjects];

		//Also copy the index value to make sure we aren't screwing up access to the other object (that is now in the slot where the deleted object used to be)
		indexArray[index].index = idx.index;

		//And null index to the deleted object
		idx.index = OBJECT_IS_EMPTY;

		//Add the handle to the free list again
		indexArray[freelist_enqueue].next = externalHandle;
		freelist_enqueue = externalHandle;
	}

private:

	/************************************************************************/
	/*							Variables                                   */
	/************************************************************************/
	IndexType maxObjects;
	const IndexType OBJECT_IS_EMPTY;
	
	IndexType activeObjects;
	IndexType freelist_enqueue, freelist_dequeue;

	IndexObject* indexArray;
	ObjectContainer objectArray;
};