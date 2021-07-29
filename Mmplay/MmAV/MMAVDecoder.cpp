#include "MmAV.h"
#include "MMAVDecoderPrivate.h"
#include "MMAVStreamPrivate.h"
#include "MmAVPacketPrivate.h"
#include "MMAVFramePrivate.h"
#include "MmAVReaderPrivate.h"
MMAVDecoder::MMAVDecoder()
{
	imp = new MMAVDecoderPrivate();
	imp->codecContext = avcodec_alloc_context3(nullptr);
	
}
MMAVDecoder::~MMAVDecoder()
{
	if (imp->codecContext != nullptr)
	{
		avcodec_free_context(&imp->codecContext);
		imp->codecContext = nullptr;
	}
	if (imp != nullptr) {
		delete imp;
		imp = nullptr;
	}
	


}
int MMAVDecoder::Init(MMAVStream* stream)
{
	timebaseNum = stream->timebaseNum;
	timebaseDen = stream->timebaseDen;
	avcodec_parameters_to_context(imp->codecContext, stream->imp->codecpar);

	const AVCodec* acCodec = avcodec_find_decoder(imp->codecContext->codec_id);
	/*
	* 没有此句会出现：Could not update timestamps for skipped samples
	pCodecCtx->pkt_timebase = pFormatCtx->streams[streamIndex]->time_base;
	*/
	//imp->codecContext->time_base = reader.imp->formatCtx->streams[streamIndex]->time_base;
	int ret=avcodec_open2(imp->codecContext,acCodec,nullptr);
	if (ret)
	{
		printf("avcodec_open2 open fail");
		return -1;
	}
	
}
int MMAVDecoder::SendPacket(MmAVPacket* pkt) {

	int ret = 0;
	if (pkt == nullptr)
	{
		int ret = avcodec_send_packet(imp->codecContext, nullptr);
	}
	else {
		ret = avcodec_send_packet(imp->codecContext, pkt->imp->pkt);
	}
	
	return ret;
}

int MMAVDecoder::RecvFrame(MMAVFrame* frame)
{
	int ret = avcodec_receive_frame(imp->codecContext, frame->imp->frame);
	//frame->VideoPrint();
	if (!ret)
	{
		//将秒级时间戳计算 赋值
		frame->imp->ptsSec = frame->imp->frame->pts * 1.0 * timebaseNum / timebaseDen;//乘1.0变double
		//printf("Pts:%11d\n", frame->imp->ptsSec);
	}
	
	
	return ret;
}

int MMAVDecoder::Close()
{
	return avcodec_close(imp->codecContext);
	
}
