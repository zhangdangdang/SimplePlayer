# CMakeList.txt: 顶层 CMake 项目文件，在此处执行全局配置
# 并包含子项目。
#
cmake_minimum_required (VERSION 3.8)

project ("Mmplay")

# 包含子项目。
add_subdirectory ("Mmplay")
add_subdirectory ("Mmthread")
add_subdirectory ("MmAV")
add_subdirectory ("MMPlayTest")
#add_subdirectory ("MMQueue")#只用头文件的不加库目录
