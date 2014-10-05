//Credits where credits are due; this class is very much based upon the "ID lookup table" data structure by Niklas Frykholm as Bitsquid:
//http://bitsquid.blogspot.se/2011/09/managing-decoupling-part-4-id-lookup.html

#pragma once

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
	//Eh, I know it's a somewhat unorthodox naming, but I feel it's descriptive.
	static const IndexType OBJECT_IS_EMPTY = sizeof(IndexType);
	static const IndexType MAX_OBJECT_LIMIT = OBJECT_IS_EMPTY-1;

private:
	struct ContainerObject
	{
		unsigned short id;
		ObjectType data;
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
	{
		maxObjects = size;
		activeObjects = 0;

		objectArray = new ContainerObject[maxObjects];
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
		delete[] objectArray;
		delete[] indexArray;
	}

	/************************************************************************/
	/*                              Operators                               */
	/************************************************************************/
	const ObjectType&	operator[](int i)	const		{ return objectArray[i].data; }
	ObjectType&			operator[](int i)				{ return objectArray[i].data; }

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
	ContainerObject* Begin() { return objectArray; }
	ContainerObject* End() { return objectArray+activeObjects; }

	ObjectType& GetSpecificObject(IndexType handle) 
	{ 
		return objectArray[indexArray[handle].index].data; 
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

			ContainerObject& object = objectArray[index.index];
			object.id = indexID;
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
		ContainerObject& object = objectArray[idx.index];

		//And overwrite the object with the currently furthest out object
		//While at the same time reducing activeObjects count with one.
		//Essentially we delete and copy in one execution
		object = objectArray[--activeObjects];

		//Also copy the index value to make sure we aren't screwing up access to the other object (that is now in the slot where the deleted object used to be)
		indexArray[object.id].index = idx.index;

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
	
	IndexType activeObjects;
	IndexType freelist_enqueue, freelist_dequeue;

	IndexObject* indexArray;
	ContainerObject* objectArray;
};