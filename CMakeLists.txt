cmake_minimum_required(VERSION 3.6)
project(PerfectPixelMess)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=c11")

set(SOURCE_FILES main.c main.h utils.c utils.h lists.c lists.h image.c image.h conv_mat.c conv_mat.h filter.c filter.h)
add_executable(ppm ${SOURCE_FILES})