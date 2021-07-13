#pragma once
#include <thread>
class Mmthread
{
public:
	virtual void run() = 0;
	int Start();
	int Stop();
	int stopFlog = 0;
private:
	//指针初始化一定要赋初值nullptr
	std::thread* t=nullptr;
	
};

