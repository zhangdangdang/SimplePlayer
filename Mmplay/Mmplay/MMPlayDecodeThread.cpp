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
	//�������
	while (!stopFlog){
		std::this_thread::sleep_for(std::chrono::milliseconds(1));//0�ǰ��̹߳���һ��ʱ��
		if (type == MMDecoderType::MMDECODER_TYPE_VIDEO)//��ֹ�ڴ�ռ�ù���
		{
			//printf("VideoQueueSize %d\n", playerCtr->GetVideoQueueSize());
			if (playerCtr->GetVideoQueueSize() > 10)//if �󲻼ӷֺ�
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
		//��pop������ѵ�һ��pkt��ֵ���ⲿ���� Ȼ��ɾ����
		int ret = packetQueue.Pop(&pkt);//
		if (ret)
		{
			//d����Ϊ��
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
				//���͸��ϲ����ʾ��
				playerCtr->GetframeH = frame->GetH();
				playerCtr->GetframeW = frame->GetW();
				//frame->VideoPrint();
				//Videoframecounts++;
				//printf("Video Decoder Success:%d\n", Videoframecounts);
			}
			else if (type == MMDecoderType::MMDECODER_TYPE_AUDIO) {
				playerCtr->PushFrameToAudioQueue(frame);
			}
			/*ʹ��pkt���������ж���ʲô��*/

			//if(pkt->GetIndex()== AVMEDIA_TYPE_VIDEO)
			//if (pkt->imp->pkt->stream_index == AVMEDIA_TYPE_VIDEO)//�Ĺ�
			//{
			//	Videoframecounts++;
			//	printf("Video Decoder Success:%d\n", Videoframecounts);
			//	//ͨ��playerCtr �ľ�������䴫������
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
