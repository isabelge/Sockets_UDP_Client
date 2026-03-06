cmake_minimum_required(VERSION 3.31)
project(Sockets_UDP)

set(CMAKE_CXX_STANDARD 20)

add_executable(Sockets_UDP main.cpp)

target_link_libraries(Sockets_UDP ws2_32)