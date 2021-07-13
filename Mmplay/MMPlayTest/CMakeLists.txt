# CMakeList.txt: Mmplay 的 CMake 项目，在此处包括源代码并定义
# 项目特定的逻辑。
#
cmake_minimum_required (VERSION 3.8)
include_directories(../)
include_directories(../3rdpart/ffmpeg/include) #添加库文件实现

link_directories(../3rdpart/ffmpeg/lib)
# 将源代码添加到此项目的可执行文件。
add_executable (MMPlayTest "Main.cpp" )
target_link_libraries(MMPlayTest Mmthread)
target_link_libraries(MMPlayTest MmAV)
target_link_libraries(MMPlayTest Mmplay)#不加这句会显示 各种符号错误 无法解析的外部符号
target_link_libraries(MMPlayTest avcodec)
target_link_libraries(MMPlayTest avdevice)
target_link_libraries(MMPlayTest avfilter)
target_link_libraries(MMPlayTest avformat)
target_link_libraries(MMPlayTest avutil)
target_link_libraries(MMPlayTest postproc)
target_link_libraries(MMPlayTest swresample)
target_link_libraries(MMPlayTest swscale)
# TODO: 如有需要，请添加测试并安装目标。
