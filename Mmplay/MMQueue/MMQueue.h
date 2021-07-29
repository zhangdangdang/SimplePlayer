#pragma once
#include<queue>
#include<mutex>
/*
MMQueue<MMAVFrame> queue
MMQueue<MMAVPacket> queue
*/
template<typename T>
class MMQueue
{
public:
	MMQueue()
	{

	}
	virtual ~MMQueue()
	{
		std::lock_guard<std::mutex> lck(mut);
		while (Size()>0)
		{
			//T * t = queue.front();
			//*t = queue.front();
			queue.pop();
			//Pop(&t);
			printf("队列MMQueue析构\n");
		}
	}
	int Push(T* t)
	{
		std::lock_guard<std::mutex> lck(mut);
		queue.push(t);

		return 0;
	}
	int Pop(T **t)
	{
		std::lock_guard<std::mutex> lck(mut);
		if (Size() > 0)
		{

			*t = queue.front();
			queue.pop();
			//printf("pop");
			return 0;
		}
		
		
		return 1;
	}
	int Size()
	{
		return queue.size();
	}
private:
	std::queue<T *> queue;
	std::mutex mut;
	//同一个线程允许重复加锁 复杂度高时不建议使用

	//std::recursive_mutex mut;
};

