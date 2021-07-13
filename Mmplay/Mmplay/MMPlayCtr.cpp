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
	//��ȡ�߳�����ʱ���ʱ�� start_time
	long long startTime = MMAVTime::GetTime();
	MMAVFrame* videoFrame = nullptr;
	MMAVFrame* audioFrame = nullptr;
	//������ͣ���
	long long sleepCountTime = 0;
	while (!stopFlog)
	{
		//����һ��ʱ�� ����cpu�ᱻ����
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
		long long sleepDTime = sleepTimeEnd - sleepTimeStart;//����sleep���
		sleepCountTime += sleepDTime;

		//��ȡ��ǰʱ�� now_time
		long long nowTime = MMAVTime::GetTime();
		//��ȡ����ǰʱ��Ϳ�ʼʱ��Ĳ�ֵ d_time
		long long dTime = nowTime - startTime;
		dTime = dTime - sleepCountTime;
		dTime = dTime + (long long)(seekTime*1000);
		//printf("dTime:%11d\n", dTime);
		 //����Ƶ��������� ��ȡһ֡��Ƶ frame_pts
		//MMAVFrame* videoFrame = nullptr;
		if (videoFrame == nullptr)
		{
			//����ȥȡ֡
			//printf("Video Queue size---------------:%d\n", videoQueue.Size());
			videoQueue.Pop(&videoFrame);//�ж��ǲ��ǿ� ��ֹ֡��û�д�����һ֡�����ˡ�
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
			//��� frame_pts>d_time

			if (videoFrame->GetPts() <= dTime)
			{
				//��֡��Ƶ Ӧ���������ų���
				//videoFrame = nullptr;
				printf("Video Frame:%11d\n", videoFrame->GetPts());
				delete videoFrame;
				videoFrame = nullptr;
			}
			else
			{
				//������֡��Ƶ���������ŵ�ʱ�򣬳�����ѡ������ȥ������Ƶ 
				//��ǰ�� �ж��ǲ��ǿ� ��ֹ֡��û�д�����һ֡�����ˡ�
				  
			}
		
		}

		
		

		 //����ƵƵ��������� ��ȡһ֡��Ƶ frame_pts
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
				//��֡��Ƶ Ӧ���������ų���
				//videoFrame = nullptr;
				//printf("Audio Frame\n");
				delete audioFrame;
				audioFrame = nullptr;
			}
			else
			{
				//������֡��Ƶ���������ŵ�ʱ�򣬳�����ѡ������ȥ������Ƶ 
				//��ǰ�� �ж��ǲ��ǿ� ��ֹ֡��û�д�����һ֡�����ˡ�

			}
		}
		//��� frame_pts>d_time
		//��֡��Ƶ Ӧ���������ų���
		//������֡��Ƶ���������ŵ�ʱ�򣬳�����ѡ�� 
	}
	readerThread.Stop();
}