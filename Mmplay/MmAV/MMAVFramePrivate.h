#pragma once

extern "C"
{
#include <libavformat/avformat.h>//���Ҳ����ļ� �Ĵ���cmakelists�Ѿ���û�����������ܳ��ڣ������ߵĿ�û�����
}//ϣ�����������߿���������ӿ� ���ص� ����MmPacketPrivate.h
class MMAVFramePrivate
{
public:


	AVFrame* frame = nullptr;
	//ʱ���
	double ptsSec = 0.0;

};

