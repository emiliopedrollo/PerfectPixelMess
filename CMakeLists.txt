cmake_minimum_required(VERSION 3.6)
project(PerfectPixelMess)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=c11")

set(SOURCE_FILES main.c)
add_executable(ppm ${SOURCE_FILES})