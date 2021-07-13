// Mmplay.h: 标准系统包含文件的包含文件
// 或项目特定的包含文件。

#pragma once

#include <iostream>
#include <string>
#include "Mmthread/Mmthread.h"
#include "MmAV/MmAV.h"
#include "MMQueue/MMQueue.h"
enum MMDecoderType
{
	MMDECODER_TYPE_VIDEO = 0,
	MMDECODER_TYPE_AUDIO = 1
};
enum MMPlayCtrStatus
{
	MMPLAYER_CTR_STATUS_PLAYING = 0,
	MMPLAYER_CTR_STATUS_PASUEING = 1
};

class MMPlayCtr:public Mmthread
{
public:
	//播放控制线程
	MMPlayCtr(double _seekTime=0.0);
	~MMPlayCtr();
	virtual void run();
	int Play();
	int Pause();

	int GetVideoQueueSize();
	int GetAudiQueueSize();

	int PushFrameToVideoQueue(MMAVFrame *frame);
	int PushFrameToAudioQueue(MMAVFrame* frame);
private:
	MMQueue<MMAVFrame> videoQueue;
	MMQueue<MMAVFrame> audioQueue;
	//信号量
	MMPlayCtrStatus status = MMPlayCtrStatus::MMPLAYER_CTR_STATUS_PLAYING;
	double seekTime = 0.0;
};



class MMPlayerReaderThread :public Mmthread
{
public:
	MMPlayerReaderThread(std::string _path, double seekTime ,MMPlayCtr *_playerCtr);
	~MMPlayerReaderThread();
	virtual void run();
private:
	std::string path;
	MMPlayCtr* playerCtr = nullptr;
	double seekTime = 0.0;
};


class MMPlayDecodeThread : public Mmthread
{
public:
	MMPlayDecodeThread (MMPlayCtr* _playerCtr, MMDecoderType _type);
	~MMPlayDecodeThread ();
	virtual void run();
	int Init(MMAVStream* avStream);
	int PutPacket(MmAVPacket* pkt);
	int GetPacketQueueSize();
private:
	MMAVDecoder* decoder = nullptr;
	MMQueue<MmAVPacket> packetQueue;
	MMPlayCtr* playerCtr = nullptr;

	MMDecoderType type;
};



class MMPlayer
{
public:
	MMPlayer(std::string _path);


	~MMPlayer();
	int Open(double time = 0);
	int Stop();
	int Play();
	int Pause();
	/*32min
	* 读到前一个I帧
	解码器清空

	*/
	int Seek(double time);
private:
	std::string path;
	MMPlayerReaderThread * readerThread=nullptr;
	MMPlayCtr* playCtr = nullptr;
};




// TODO: 在此处引用程序需要的其他标头。
