cmake_minimum_required(VERSION 3.5)
project(ass3)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES a3search.c)
add_executable(ass3 ${SOURCE_FILES})