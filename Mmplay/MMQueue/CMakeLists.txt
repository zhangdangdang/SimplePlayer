# CMakeList.txt: 顶层 CMake 项目文件，在此处执行全局配置
# 并包含子项目。
#
cmake_minimum_required (VERSION 3.8) 

include_directories(../3rdpart/ffmpeg/include) #添加库文件实现

add_library(# 添加头文件和 源文件，若cpp文件缺失，会报错，可以补一个空白的文件
			#MMQueue.h
			#MMQueue.cpp
            )



