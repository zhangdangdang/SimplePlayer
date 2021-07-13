#include <thread>
#include <iostream>
#include "Mmthread.h"

int Mmthread:: Start()
{
	if (t == nullptr)
	{
		stopFlog = 0;
		t = new std::thread(&Mmthread::run, this);
	}
	//std::thread t(&Mmthread::run, this);
	//t.detach();
	return 0;
}
int Mmthread::Stop()
{

	if (t != nullptr)
	{
		stopFlog = 1;
		t->join();
		delete t;
		t = nullptr;
	}
	return 0;
}