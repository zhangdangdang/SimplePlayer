#include "MmAV.h"
#include "MMAVFramePrivate.h"
extern "C"
{
#include<libavutil/pixdesc.h>
}
MMAVFrame::MMAVFrame()
{
	imp = new MMAVFramePrivate();
	imp->frame = av_frame_alloc();

}
MMAVFrame::~MMAVFrame()
{
	if (imp->frame != nullptr)
	{
		av_frame_free(&imp->frame);
		imp->frame = nullptr;
	}
	if (imp!= nullptr)
	{
		delete imp;
		imp = nullptr;
	}
}
int MMAVFrame::VideoPrint()
{
	printf("width:%d\n", imp->frame->width);
	printf("height:%d\n", imp->frame->height);
	AVPixelFormat format = (AVPixelFormat)(imp->frame->format);
	char* str = (char*)malloc(128);
	str = av_get_pix_fmt_string(str, 128, format);
	printf("Pixel Format: %s\n", str);
	free(str);

	for (int i = 0; i < AV_NUM_DATA_POINTERS; ++i)
	{
		//printf("VideoLinesize[%d]:%d\n", i,imp->frame->linesize[i]);
	}
	return 0;
}
int MMAVFrame::AudioPrint()
{
	int channel = imp->frame->channels;
	printf("channels:%d\n", channel);
	printf("nb_samples:%d\n", imp->frame->nb_samples);
	printf("sample_rate:%d\n", imp->frame->sample_rate);
	AVSampleFormat format = (AVSampleFormat)(imp->frame->format);
	
	char* str = (char*)malloc(128);

	str = av_get_sample_fmt_string(str, 128, format);

	printf("Pixel Format: %s\n", str);
	
	for (int i = 0; i < AV_NUM_DATA_POINTERS; i++)
	{
		printf("AudioLinesize[%d]:%d\n", i, imp->frame->linesize[i]);
	}
	free(str);
	return 0;
}
int MMAVFrame::GetW()
{
	return imp->frame->width;
}
int MMAVFrame::GetH() {
	return imp->frame->height;
}
int MMAVFrame::GetY(unsigned char* y)
{
	int wideth = GetW();
	int heigth = GetH();
	//memcpy(y, imp->frame->data[0], wideth*heigth);
	for (int i = 0; i < heigth; i++) {
		memcpy(y + i * wideth, imp->frame->data[0] + i * imp->frame->linesize[0], wideth);

	}
	return 0;
}
int MMAVFrame::GetU(unsigned char* u)
{
	int wideth = GetW()/2;
	int heigth = GetH()/2;
	//memcpy(y, imp->frame->data[0], wideth*heigth);
	for (int i = 0; i < heigth; i++) {
		memcpy(u + i * wideth, imp->frame->data[1] + i * imp->frame->linesize[1], wideth);

	}
	return 0;
}
int MMAVFrame::GetV(unsigned char* v)
{
	int wideth = GetW()/2;
	int heigth = GetH()/2;
	//memcpy(y, imp->frame->data[0], wideth*heigth);
	for (int i = 0; i < heigth; i++) {
		memcpy(v + i * wideth, imp->frame->data[2] + i * imp->frame->linesize[2], wideth);

	}
	return 0;
}
int MMAVFrame::GetLinesize() {
	
	return imp->frame->linesize[0];
}
int MMAVFrame::GetAudio(unsigned char* a)
{
	int linesize = GetLinesize();
	memcpy(a, imp->frame->data[0], linesize);
	return 0;
}

long long MMAVFrame::GetPts()
{

	//printf("ptsSec:%f\n", imp->ptsSec);
	//在decoder里填充的数据
	return (long long)(imp->ptsSec*1000);//秒级转化为毫秒
}