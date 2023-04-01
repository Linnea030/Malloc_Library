cmake_minimum_required(VERSION 3.16)
project(untitled C)

set(CMAKE_C_STANDARD 99)

add_executable(untitled my_malloc.c cmake-build-debug/my_malloc.c cmake-build-debug/my_malloc.h my_malloc.c my_malloc.h)
