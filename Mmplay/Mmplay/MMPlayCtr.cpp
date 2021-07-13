#include "Mmplay.h"
#include "3rdpart/SDL2-2.0.14/include/SDL.h"

MMPlayCtr::MMPlayCtr(double _seekTime)
{
	seekTime = _seekTime;
}
MMPlayCtr::~MMPlayCtr()
{

}
int MMPlayCtr::GetVideoQueueSize()
{

	return videoQueue.Size();
}
int MMPlayCtr::GetAudiQueueSize()
{

	return audioQueue.Size();
}
int MMPlayCtr::PushFrameToVideoQueue(MMAVFrame* frame)
{
	
	return videoQueue.Push(frame);
}
int MMPlayCtr::PushFrameToAudioQueue(MMAVFrame* frame)
{
	
	return audioQueue.Push(frame);
}
int MMPlayCtr::Play()
{
	status = MMPlayCtrStatus::MMPLAYER_CTR_STATUS_PLAYING;
	return 0;
}
int MMPlayCtr::Pause()
{
	status = MMPlayCtrStatus::MMPLAYER_CTR_STATUS_PASUEING;
	return 0;
}

void MMPlayCtr::run()
{
	MMPlayerReaderThread  readerThread("E://ffmpegcode//qiao//3 chapter3//Mmplay//Mmplay//girls.mp4", seekTime,this);
	readerThread.Start();
	//获取线程启动时候的时间 start_time
	long long startTime = MMAVTime::GetTime();
	MMAVFrame* videoFrame = nullptr;
	MMAVFrame* audioFrame = nullptr;
	//可能暂停多次
	long long sleepCountTime = 0;
	while (!stopFlog)
	{
		//挂起一段时间 否则cpu会被跑满
		std::this_thread::sleep_for(std::chrono::milliseconds(1));

		/*if (status == MMPlayCtrStatus::MMPLAYER_CTR_STATUS_PASUEING)
		{
			continue;
		}*/
		long long sleepTimeStart = MMAVTime::GetTime();
		while (status == MMPlayCtrStatus::MMPLAYER_CTR_STATUS_PASUEING)
		{

			std::this_thread::sleep_for(std::chrono::milliseconds(1));
		}
		long long sleepTimeEnd = MMAVTime::GetTime();
		long long sleepDTime = sleepTimeEnd - sleepTimeStart;//可能sleep多次
		sleepCountTime += sleepDTime;

		//获取当前时间 now_time
		long long nowTime = MMAVTime::GetTime();
		//获取到当前时间和开始时间的差值 d_time
		long long dTime = nowTime - startTime;
		dTime = dTime - sleepCountTime;
		dTime = dTime + (long long)(seekTime*1000);
		//printf("dTime:%11d\n", dTime);
		 //从视频缓存队列中 获取一帧视频 frame_pts
		//MMAVFrame* videoFrame = nullptr;
		if (videoFrame == nullptr)
		{
			//尝试去取帧
			//printf("Video Queue size---------------:%d\n", videoQueue.Size());
			videoQueue.Pop(&videoFrame);//判断是不是空 防止帧还没有处理，下一帧就来了。
		}
		//printf("Video Queue Size--------------------------------------:%d\n", videoQueue.Size());

		if (videoFrame != nullptr) {
			if (videoFrame->GetPts() < (long long)(seekTime * 1000))
			{

				delete videoFrame;
				videoFrame = nullptr;
			}
		}
		if (videoFrame != nullptr)
		{
			//如果 frame_pts>d_time

			if (videoFrame->GetPts() <= dTime)
			{
				//这帧视频 应该立即播放出来
				//videoFrame = nullptr;
				printf("Video Frame:%11d\n", videoFrame->GetPts());
				delete videoFrame;
				videoFrame = nullptr;
			}
			else
			{
				//否则这帧视频还不到播放的时候，程序自选，或者去处理音频 
				//在前边 判断是不是空 防止帧还没有处理，下一帧就来了。
				  
			}
		
		}

		
		

		 //从音频频缓存队列中 获取一帧视频 frame_pts
		if (audioFrame == nullptr)
		{
			audioQueue.Pop(&audioFrame);
		 }
		if (audioFrame != nullptr)
		{
			if (audioFrame->GetPts() < (long long)seekTime * 1000)
			{

				delete audioFrame;
				audioFrame = nullptr;
			}
		}

		if (audioFrame != nullptr)
		{
			if (audioFrame->GetPts() <= dTime)
			{
				printf("audioFrame :%11d\n", audioFrame->GetPts());
				//这帧音频 应该立即播放出来
				//videoFrame = nullptr;
				//printf("Audio Frame\n");
				delete audioFrame;
				audioFrame = nullptr;
			}
			else
			{
				//否则这帧视频还不到播放的时候，程序自选，或者去处理音频 
				//在前边 判断是不是空 防止帧还没有处理，下一帧就来了。

			}
		}
		//如果 frame_pts>d_time
		//这帧视频 应该立即播放出来
		//否则这帧视频还不到播放的时候，程序自选。 
	}
	readerThread.Stop();
}