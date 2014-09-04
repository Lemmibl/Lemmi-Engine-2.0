#pragma once
#include <queue>
#include <tinythread.h>

//This class is based on an excellent article found here: http://www.justsoftwaresolutions.co.uk/threading/implementing-a-thread-safe-queue-using-condition-variables.html
//So props to them!
template<typename Data>
class ThreadsafeQueue
{
private:
	using namespace tthread;

public:
	~ThreadsafeQueue()
	{
		Shutdown();
	}

	void Shutdown()
	{
		lock_guard<mutex> guard(mutexVariable);
		conditionVariable.notify_all();
	}

	void Push(const Data & data)
	{
		lock_guard<mutex> guard(mutexVariable);
		queue.push(data);
		conditionVariable.notify_one();
	}

	//std::move version
	void Push(Data&& data)
	{
		lock_guard<mutex> guard(mutexVariable);
		queue.push(std::move(data));
		conditionVariable.notify_one();
	}

	bool Try_pop(Data& popped_value)
	{
		lock_guard<mutex> guard(mutexVariable);
		if(queue.empty())
		{
			return false;
		}

		popped_value = queue.front();
		queue.pop();

		return true;
	}

	void Wait_and_pop(Data& popped_value)
	{
		lock_guard<mutex> guard(mutexVariable);

		while(queue.empty())
		{
			conditionVariable.wait(mutexVariable);
		}

		popped_value = queue.front();
		queue.pop();
	}

	//Unfortunately, std::queue doesn't have a built-in clear function, so this will have to do.
	void Clear()
	{
			lock_guard<mutex> guard(mutexVariable);

			//Copy and swap idiom to clear out queue memory
			std::queue<Data>().swap(queue);
	}

	bool Empty() const
	{
		lock_guard<mutex> guard(mutexVariable);
		return queue.empty();
	}

	size_t Size()
	{ 
		lock_guard<mutex> guard(mutexVariable);
		return queue.size(); 
	}

private:
	condition_variable conditionVariable;
	mutable mutex mutexVariable;
	std::queue<Data> queue;
};