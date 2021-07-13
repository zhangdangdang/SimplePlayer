#include <iostream>
#include"MmAV.h"
#include "MmAVPacketPrivate.h"
extern "C"
{
#include <libavformat/avformat.h>
}

MmAVPacket::MmAVPacket()
{
	imp = new MmAVPacketPrivate();
	imp->pkt=av_packet_alloc();
}

MmAVPacket::~MmAVPacket()
{
	if (imp->pkt != nullptr)
	{
		av_packet_free(&imp->pkt);
		imp->pkt = nullptr;
	}
	if (imp != nullptr) {
		delete imp;
		imp = nullptr;
	}
}
int MmAVPacket::GetIndex()
{
	return imp->pkt->stream_index;
}