#pragma once
#include <thread>
class Mmthread
{
public:
	virtual void run() = 0;
	int Start();
	int Stop();
	int stopFlog = 0;
	virtual ~Mmthread()
	{

	}
private:
	//ָ���ʼ��һ��Ҫ����ֵnullptr
	std::thread* t=nullptr;
	
};

