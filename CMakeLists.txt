cmake_minimum_required(VERSION 3.24)
project(Tiny_KV_DB)

set(CMAKE_CXX_STANDARD 17)

add_executable(Tiny_KV_DB main.cpp)

add_subdirectory(storage)
add_subdirectory(data_structure)
add_subdirectory(server)
add_subdirectory(client)