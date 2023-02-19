cmake_minimum_required(VERSION 3.24)
project(Tiny_KV_DB)

set(CMAKE_CXX_STANDARD 17)

add_executable(data_structure main.cpp linked_list.h int_set.h int_set.cpp)
include_directories(.)