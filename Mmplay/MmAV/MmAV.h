#pragma once
#define __STDC_CONSTANT_MACROS
//
//extern "C"
//{
//#include <libavformat/avformat.h>//报找不到文件 的错误，cmakelists已经配好环境，问题可能出在，引用者的库没有配好
//}//希望其他调用者看不到这个接口 隐藏掉 创建MmPacketPrivate.h
class MmAVPacketPrivate;
class MmAVPacket
{
public:
	MmAVPacket();
	~MmAVPacket();
	int GetIndex();
	//AVPacket* pkt = nullptr;
	MmAVPacketPrivate* imp = nullptr;

private:

};
class MMAVStreamPrivate;
class MMAVStream
{
public:
	MMAVStream();
	~MMAVStream();
	int streamIndex = -1;
	int timebaseNum = 0;
	int timebaseDen = 0;
	MMAVStreamPrivate* imp = nullptr;
private:

};

//创建图像缓存
class MMAVFramePrivate;
class MMAVFrame
{
public:
	MMAVFrame();
	~MMAVFrame();
	MMAVFramePrivate* imp = nullptr;
	int GetW();
	int GetH();
	int GetY(unsigned char * y);
	int GetU(unsigned char * u);
	int GetV(unsigned char * v);
	int GetLinesize();
	int GetAudio(unsigned char* a);
	//打印video属性
	int VideoPrint();
	//打印audio属性 返回linesize数值
	int AudioPrint();
	long long GetPts();

private:

};

//解码缓存
class MMAVDecoderPrivate;
class MMAVDecoder
{
public:
	MMAVDecoder();
	~MMAVDecoder();
	//查找编码器初始化时间基 
	int Init(MMAVStream* stream);
	int SendPacket(MmAVPacket* pkt);
	int RecvFrame(MMAVFrame* frame);
	int Close();
	MMAVDecoderPrivate* imp = nullptr;

private:
	int timebaseNum = 0;
	int timebaseDen = 0;
	//MMAVFrame *deframe = nullptr;
};
class MmAVReaderPrivate;
class MmAVReader
{
public:
	MmAVReader();
	~MmAVReader();
	int Open(const char* path);
	int Close();
	/**
   返回流数量
    */
	int GetStreamCount();
	/*return 0 success
	判断是否音视频流

	*/
	int GetStream(MMAVStream *stream,int streamId);
	//得到视频流的序号
	int GetVideoStreanIndex();
	int GetAudioStreanIndex();
	int Read(MmAVPacket* packet);
	int Seek(double time);
	//MmAVPacketPrivate* pformatCtx = nullptr;
	MmAVReaderPrivate* imp = nullptr;
private:
	/*AVFormatContext* formatCtx = nullptr;*/
	
};

class MMAVTime
{
public:
	MMAVTime();
	~MMAVTime();
	static long long GetTime();
private:

};





