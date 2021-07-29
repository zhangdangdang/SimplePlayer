#include "Mmplay/Mmplay.h"
#include <Windows.h>
#include<chrono>
std::int16_t get_mill_time_stamp() {
	std::chrono::time_point<std::chrono::system_clock, std::chrono::milliseconds> tp = std::chrono::time_point_cast<std::chrono::milliseconds>(std::chrono::system_clock::now());
		auto tmp = std::chrono::duration_cast<std::chrono::milliseconds>(tp.time_since_epoch());
		return tmp.count();
}
/*
现在可以根据pts来计算一桢在整个视频中的时间位置：

timestamp(秒) = pts * av_q2d(st->time_base)
这里的st是一个AVStream对象指针
*/

int main(int argc, char* argv[])
{
	long long t = MMAVTime::GetTime();

	printf("t:%Id\n",t);//%11d
	MMPlayer player("E://ffmpegcode//qiao//3 chapter3//Mmplay//Mmplay//girls.mp4");
	int ret = player.Open();
	if (ret)
	{
		printf("Player Open Fail\n");
		return -1;
	}
	//std::this_thread::sleep_for(std::chrono::seconds(6));
	player.Play();
	printf("===========================Play =============================\n");
	//..wait
	std::this_thread::sleep_for(std::chrono::seconds(3));
	//player.Pause();
	

	//std::this_thread::sleep_for(std::chrono::seconds(1));

	player.Seek(10);
	//player.Play();
	printf("===========================seek =============================\n");
	std::this_thread::sleep_for(std::chrono::seconds(10));
	//
	//std::this_thread::sleep_for(std::chrono::seconds(1));
	player.Pause();
	printf("===========================Pause =============================\n");

	//std::this_thread::sleep_for(std::chrono::seconds(1));
	//player.Play();
	//printf("sleep\n");
	////..wait
	std::this_thread::sleep_for(std::chrono::seconds(60*60));
	player.Stop();
	return 0;
}