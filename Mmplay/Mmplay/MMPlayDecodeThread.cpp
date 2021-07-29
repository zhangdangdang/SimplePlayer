#include "Mmplay/Mmplay.h"
#include "MmAV/MmAVPacketPrivate.h"
MMPlayDecodeThread::MMPlayDecodeThread(MMPlayCtr* _playerCtr, MMDecoderType _type)
{
	decoder = new MMAVDecoder();
	playerCtr = _playerCtr;
	type = _type;
}
MMPlayDecodeThread::~MMPlayDecodeThread()
{
	if (decoder != nullptr)
	{
		delete decoder;
		decoder = nullptr;
	}
}
int MMPlayDecodeThread::PutPacket(MmAVPacket* pkt)
{
	return packetQueue.Push(pkt);
	
}
int MMPlayDecodeThread::GetPacketQueueSize()
{
	return packetQueue.Size();
}

void MMPlayDecodeThread::run()
{
	int Videoframecounts = 0;
	int Audioframecounts = 0;
	//解码操作
	while (!stopFlog){
		std::this_thread::sleep_for(std::chrono::milliseconds(1));//0是把线程挂起一点时间
		if (type == MMDecoderType::MMDECODER_TYPE_VIDEO)//防止内存占用过大
		{
			//printf("VideoQueueSize %d\n", playerCtr->GetVideoQueueSize());
			if (playerCtr->GetVideoQueueSize() > 10)//if 后不加分号
			{
				continue;
			}
			//printf("Push video\n");
		}
		else if (type == MMDecoderType::MMDECODER_TYPE_AUDIO)
		{
			//printf("AudiQueueSize %d\n", playerCtr->GetAudiQueueSize());
			if (playerCtr->GetAudiQueueSize() > 20)
			{
				continue;
			}
		}

		MmAVPacket* pkt = nullptr;//
		//先pop函数里把第一个pkt赋值给外部变量 然后删掉。
		int ret = packetQueue.Pop(&pkt);//
		if (ret)
		{
			//d队列为空
			//std::this_thread::sleep_for(std::chrono::milliseconds(1));
			continue;
		}
		

		decoder->SendPacket(pkt);
		while (1)
		{
			MMAVFrame *frame = new MMAVFrame();
			ret = decoder->RecvFrame(frame);
			if (ret)
			{
				break;
			}
			if (type == MMDecoderType::MMDECODER_TYPE_VIDEO) {
				playerCtr->PushFrameToVideoQueue(frame);
				//传送给上层给显示用
				playerCtr->GetframeH = frame->GetH();
				playerCtr->GetframeW = frame->GetW();
				//frame->VideoPrint();
				//Videoframecounts++;
				//printf("Video Decoder Success:%d\n", Videoframecounts);
			}
			else if (type == MMDecoderType::MMDECODER_TYPE_AUDIO) {
				playerCtr->PushFrameToAudioQueue(frame);
			}
			/*使用pkt的数据来判断是什么流*/

			//if(pkt->GetIndex()== AVMEDIA_TYPE_VIDEO)
			//if (pkt->imp->pkt->stream_index == AVMEDIA_TYPE_VIDEO)//改过
			//{
			//	Videoframecounts++;
			//	printf("Video Decoder Success:%d\n", Videoframecounts);
			//	//通过playerCtr 的句柄，向其传入数据
			//	//playerCtr
			//	if (type == MMDecoderType::MMDECODER_TYPE_VIDEO)
			//	{
			//		playerCtr->PushFrameToVideoQueue(frame);
			//		//printf("Push video\n");
			//	}
			//	/*else if (type == MMDecoderType::MMDECODER_TYPE_AUDIO)
			//	{
			//		playerCtr->PushFrameToAudioQueue(frame);
			//		printf("Push Audio\n");
			//	}*/
			//}
			//else if(pkt->imp->pkt->stream_index == AVMEDIA_TYPE_AUDIO)
			//{
			//	Audioframecounts++;
			//	printf("Audio Decoder Success:%d\n", Audioframecounts);
			//    if (type == MMDecoderType::MMDECODER_TYPE_AUDIO)
			//	{
			//	playerCtr->PushFrameToAudioQueue(frame);
			//	//printf("Push Audio\n");
			//	}
			//}
			
			//std::this_thread::sleep_for(std::chrono::milliseconds(1));

		}

	}
}
int MMPlayDecodeThread::Init(MMAVStream* avStream)
{
	return decoder->Init(avStream);
}
