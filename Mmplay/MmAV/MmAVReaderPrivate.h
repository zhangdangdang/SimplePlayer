#pragma once

extern "C"
{
#include <libavformat/avformat.h>//报找不到文件 的错误，cmakelists已经配好环境，问题可能出在，引用者的库没有配好
}//希望其他调用者看不到这个接口 隐藏掉 创建MmPacketPrivate.h
class MmAVReaderPrivate
{
public:

	
	AVFormatContext* formatCtx = nullptr;


};

