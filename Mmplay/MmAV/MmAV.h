#pragma once
#define __STDC_CONSTANT_MACROS
//
//extern "C"
//{
//#include <libavformat/avformat.h>//���Ҳ����ļ� �Ĵ���cmakelists�Ѿ���û�����������ܳ��ڣ������ߵĿ�û�����
//}//ϣ�����������߿���������ӿ� ���ص� ����MmPacketPrivate.h
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

//����ͼ�񻺴�
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
	//��ӡvideo����
	int VideoPrint();
	//��ӡaudio���� ����linesize��ֵ
	int AudioPrint();
	long long GetPts();

private:

};

//���뻺��
class MMAVDecoderPrivate;
class MMAVDecoder
{
public:
	MMAVDecoder();
	~MMAVDecoder();
	//���ұ�������ʼ��ʱ��� 
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
   ����������
    */
	int GetStreamCount();
	/*return 0 success
	�ж��Ƿ�����Ƶ��

	*/
	int GetStream(MMAVStream *stream,int streamId);
	//�õ���Ƶ�������
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





