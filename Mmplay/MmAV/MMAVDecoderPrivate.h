#pragma once

extern "C"
{
#include <libavformat/avformat.h>//���Ҳ����ļ� �Ĵ���cmakelists�Ѿ���û�����������ܳ��ڣ������ߵĿ�û�����
#include "libavcodec/avcodec.h"
#include "libswscale/swscale.h"
#include "libavutil/imgutils.h"
}//ϣ�����������߿���������ӿ� ���ص� ����MmPacketPrivate.h
class MMAVDecoderPrivate
{
public:
	//MmAVPacketPrivate();
	//~MmAVPacketPrivate();//���ܼӹ������������
	AVCodecContext* codecContext = nullptr;
	//AVFormatContext* formatCtx = nullptr;
private:

};

