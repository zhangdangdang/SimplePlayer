#include <iostream>
#include"MmAV.h"
#include "MmAVPacketPrivate.h"
#include "MmAVReaderPrivate.h"
#include "MMAVStreamPrivate.h"
#include "MMAVDecoderPrivate.h"
//这里不要加 ffmpeg的include
//这里不要加 ffmpeg的include
MmAVReader::MmAVReader()
{
	imp = new MmAVReaderPrivate();
	imp->formatCtx= avformat_alloc_context();
}
MmAVReader::~MmAVReader()
{
	if (imp->formatCtx != nullptr) {
		avformat_free_context(imp->formatCtx);
		imp->formatCtx = nullptr;
	}
	if (imp != nullptr)
	{
		delete imp;
		imp = nullptr;
	}
}
//return 0 on success, a negative AVERROR on failure.
int MmAVReader::Open(const char* path)
{
	if (imp->formatCtx == nullptr) {
		return -1;
	}
	if (path == nullptr) {
		return -1;
	}
	int ret=avformat_open_input(&imp->formatCtx,path,nullptr, nullptr);

	if (!ret) {
		avformat_find_stream_info(imp->formatCtx,nullptr);
		//av_dump_format(imp->formatCtx, 0, path, 0);
		return ret;
	}

	return ret;
}
//int MmAVReader::GetStream(MMAVStream* avStream, int streamId, MmAVReader *reader,MMAVDecoder *decoder)
//{
//	AVStream* ffmpegstream=imp->formatCtx->streams[streamId];
//	avStream->streamIndex = ffmpegstream->index;
//	//decoder->imp->codecContext->time_base = imp->formatCtx->streams[streamId]->time_base;
//	avcodec_parameters_copy(avStream->imp->codecpar, ffmpegstream->codecpar);
//	decoder->imp->codecContext->pkt_timebase = reader->imp->formatCtx->streams[streamId]->time_base;
//	//avStream->imp->codecpar=
//	if (avStream->imp->codecpar->codec_type == AVMEDIA_TYPE_VIDEO)//解码器参数指向类型
//	//if (imp->formatCtx->streams[streamId]->codecpar->codec_type == AVMEDIA_TYPE_VIDEO)
//	{
//		printf("it is video\n");
//	}
//	else
//	{
//		printf("it is audio\n");
//	}
//	return 0;
//}
int MmAVReader::GetStream(MMAVStream* avStream, int streamId)
{
	AVStream* ffmpegstream = imp->formatCtx->streams[streamId];
	//ffmpegstream->time_base

	avStream->timebaseNum = ffmpegstream->time_base.num;
	avStream->timebaseDen = ffmpegstream->time_base.den;
	avStream->streamIndex = ffmpegstream->index;
	//avStream->streamIndex = imp->formatCtx->streams[streamId]->index;
	//decoder->imp->codecContext->time_base = imp->formatCtx->streams[streamId]->time_base;
	//avcodec_parameters_copy(avStream->imp->codecpar, ffmpegstream->codecpar);
	avcodec_parameters_copy(avStream->imp->codecpar, imp->formatCtx->streams[streamId]->codecpar);
	//avStream->imp->codecpar=
	if (avStream->imp->codecpar->codec_type == AVMEDIA_TYPE_VIDEO)//解码器参数指向类型
	//if (imp->formatCtx->streams[streamId]->codecpar->codec_type == AVMEDIA_TYPE_VIDEO)
	{
		printf("readcpp it is video\n");
	}
	else
	{
		printf("readcpp it is audio\n");
	}
	return 0;
}
int MmAVReader::GetVideoStreanIndex() {

	int ret = av_find_best_stream(imp->formatCtx, AVMEDIA_TYPE_VIDEO, -1, -1, nullptr, 0);
	if (ret < 0)
	{
		return -1;
	}
	/*for (int i = 0; i < imp->formatCtx->nb_streams; ++i)
	{
		if (imp->formatCtx->streams[i]->codecpar->codec_type == AVMEDIA_TYPE_VIDEO)
			return i;
	}*/
	return ret;
}
int MmAVReader::GetAudioStreanIndex()
{
	int ret = av_find_best_stream(imp->formatCtx, AVMEDIA_TYPE_AUDIO, -1, -1, nullptr, 0);
	if (ret < 0)
	{
		return -1;
	}
	return ret;
}
/**
返回流数量
 */
int MmAVReader::GetStreamCount()
{
	return imp->formatCtx->nb_streams;
}


int MmAVReader::Close()
{
	if (imp == nullptr) {
		return -1;
	}
	avformat_close_input(&imp->formatCtx);
	return 0;
}
int MmAVReader::Read(MmAVPacket* packet)
{
	if (imp->formatCtx == nullptr) {
		return -1;
	}
	int ret = av_read_frame(imp->formatCtx, packet->imp->pkt);
	return ret;
}

int MmAVReader::Seek(double time)
{
	if (imp->formatCtx == nullptr) {
		return -1;
	}
	int64_t timestamp = (int64_t)(time * AV_TIME_BASE);
	av_seek_frame(imp->formatCtx,-1, timestamp,AVSEEK_FLAG_BACKWARD);
	return 0;
}

