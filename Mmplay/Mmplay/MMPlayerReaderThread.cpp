#include "Mmplay.h"
#include"MmAV/MmAV.h"

MMPlayerReaderThread::MMPlayerReaderThread(std::string _path, double _seekTime, MMPlayCtr* _playerCtr) {
	path = _path;
	playerCtr = _playerCtr;
	seekTime = _seekTime;
}
MMPlayerReaderThread::~MMPlayerReaderThread() {

}
 void MMPlayerReaderThread::run()
{
	 //printf("Run\n");
	 MmAVReader reader;
	 //open 填充formatCtx
	 int ret = reader.Open(path.c_str());
	 if (ret)
	 {
		 return ;
	 }
	 reader.Seek(seekTime);
	 int videoStreamIndex = reader.GetVideoStreanIndex();
	 int audioStreamIndex = reader.GetAudioStreanIndex();

	 //TO DO初始化解码器
	 MMPlayDecodeThread* videoDecoderThread = new MMPlayDecodeThread(playerCtr,MMDecoderType::MMDECODER_TYPE_VIDEO);//通过构造的时候把类型传进来
	 MMPlayDecodeThread* audioDecoderThread = new MMPlayDecodeThread(playerCtr, MMDecoderType::MMDECODER_TYPE_AUDIO);
	 //video
	 MMAVStream videoStream;
	 //把stream送进reader进行数据填充，对stream的编码器信息进行填充
	 reader.GetStream(&videoStream, videoStreamIndex);
	 //找到解码器初始化 
	 videoDecoderThread->Init(&videoStream);
	 //audio
	 MMAVStream audioStream;
	 reader.GetStream(&audioStream, audioStreamIndex);
	 audioDecoderThread->Init(&audioStream);

	 videoDecoderThread->Start();
	 audioDecoderThread->Start();
	 while (!stopFlog)
	 {
		 if (videoDecoderThread->GetPacketQueueSize() > 5&& audioDecoderThread->GetPacketQueueSize()>5)
		 {
			 //std::this_thread::sleep_for(std::chrono::milliseconds(1));
			 //printf("Read 自旋\n");
			 continue;
		 }
		 //std::this_thread::sleep_for(std::chrono::seconds(1));
		 MmAVPacket* pkt = new MmAVPacket();
		 int ret = reader.Read(pkt);
		 if (ret)
		 {
			 delete pkt;
			 pkt = nullptr;
			 break;
		 }
		 if (pkt->GetIndex() == videoStreamIndex)
		 {
			 videoDecoderThread->PutPacket(pkt);
			 //printf("put Videopack:%d\n", videoDecoderThread->GetPacketQueueSize());
		 }
		 if (pkt->GetIndex() == audioStreamIndex)
		 {
			 audioDecoderThread->PutPacket(pkt);
			 //printf("put Audiopack:%d\n", audioDecoderThread->GetPacketQueueSize());
		 }
		 //将Packet 放入队列缓存
		 //printf("success read pack\n");
		 //delete pkt;
		 //pkt = nullptr;
	 }
	 videoDecoderThread->Stop();
	 audioDecoderThread->Stop();
	 reader.Close();
}