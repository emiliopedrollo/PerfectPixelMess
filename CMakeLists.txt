cmake_minimum_required(VERSION 3.6)
project(PerfectPixelMess)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=c11 -ansi")

set(SOURCE_FILES main.c utils.c utils.h lists.c lists.h image.c image.h conv_mat.c conv_mat.h filter.c filter.h)
add_executable(ppm ${SOURCE_FILES})

target_link_libraries(ppm m) # -lm , para usar math.h
