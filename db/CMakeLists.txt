cmake_minimum_required(VERSION 3.24)
project(Tiny_KV_DB)

set(CMAKE_CXX_STANDARD 17)

add_executable(db main.cpp LRUCache.h dbObject.h)