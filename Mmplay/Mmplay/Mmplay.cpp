#include "Mmplay.h"

MMPlayer::MMPlayer(std::string _path)
{
	path = _path;

}

MMPlayer::~MMPlayer() {

}
int MMPlayer::Open(double time)
{
	//if (readerThread == nullptr) 
	//{
	//	readerThread = new MMPlayerReaderThread(path);
	//	readerThread->Start();
	//	return 0;
	//}

	if (playCtr == nullptr)
	{
		playCtr = new MMPlayCtr(time);
		playCtr->Start();
		return 0;
	}
	return -1;
}
int MMPlayer::Stop()
{
	//if (readerThread != nullptr)
	//{
	//	printf("stop\n");
	//	readerThread->Stop();
	//	delete readerThread;
	//	readerThread = nullptr;
	//	return 0;
	//}
	if (playCtr != nullptr)
	{
		playCtr->Stop();
		delete playCtr;
		playCtr = nullptr;
		return 0;
	}

	return -1;
		
}
int MMPlayer::Play()
{
	if (playCtr != nullptr)
	{
		playCtr->Play();
	}
	return 0;
}
int MMPlayer::Pause()
{
	if (playCtr != nullptr)
	{
		playCtr->Pause();
	}
	return 0;
}
int MMPlayer::Seek(double time)
{
	Stop();
	Open(time);
	return 0;
}