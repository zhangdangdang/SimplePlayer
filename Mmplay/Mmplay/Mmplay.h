// Mmplay.h: 标准系统包含文件的包含文件
// 或项目特定的包含文件。

#pragma once

#include <iostream>
#include <string>
#include "Mmthread/Mmthread.h"
#include "MmAV/MmAV.h"
#include "MMQueue/MMQueue.h"
#include"MmAV/MMAVFramePrivate.h"
extern "C"
{
#include "3rdpart/SDL2/include/SDL.h"
#include <libavformat/avformat.h>//报找不到文件 的错误，cmakelists已经配好环境，问题可能出在，引用者的库没有配好
#include "libavcodec/avcodec.h"
#include "libswscale/swscale.h"
#include "libavutil/imgutils.h"
//希望其他调用者看不到这个接口 隐藏掉 创建MmPacketPrivate.h
};

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
class MMSDL
{
public:
	MMSDL(int _screen_w, int _screen_h);
	~MMSDL();
	int MMSDLInit();
	int MMSdlUpdateTexture(MMAVFrame *frame, int x, int y, int w, int h);
private:
	int screen_w = 0, screen_h = 0;
	SDL_Renderer* sdlRenderer = nullptr;
	SDL_Texture* sdlTexture = nullptr;
	SDL_Rect sdlRect;
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
	int GetframeW=0;
	int GetframeH=0;
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
	bool one = true;
	struct SwsContext* img_convert_ctx = nullptr;
	MMAVFrame* PlayFrame = nullptr;
	uint8_t* out_buffer = nullptr;
	//MMSDL * SdlPlay = nullptr;
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
