# CMakeList.txt: Mmplay 的 CMake 项目，在此处包括源代码并定义
# 项目特定的逻辑。
#
cmake_minimum_required (VERSION 3.8)
include_directories(../)
include_directories(../3rdpart/ffmpeg/include) #添加库文件实现

link_directories(../3rdpart/ffmpeg/lib)
# 将源代码添加到此项目的可执行文件。
add_library (Mmplay "Mmplay.cpp" "Mmplay.h" "MMPlayerReaderThread.cpp" "MMPlayDecodeThread.cpp" "MMPlayCtr.cpp")# 加 add_executable可执行文件
target_link_libraries(Mmplay Mmthread)
target_link_libraries(Mmplay MmAV)

target_link_libraries(Mmplay avcodec)
target_link_libraries(Mmplay avdevice)
target_link_libraries(Mmplay avfilter)
target_link_libraries(Mmplay avformat)
target_link_libraries(Mmplay avutil)
target_link_libraries(Mmplay postproc)
target_link_libraries(Mmplay swresample)
target_link_libraries(Mmplay swscale)
# TODO: 如有需要，请添加测试并安装目标。
