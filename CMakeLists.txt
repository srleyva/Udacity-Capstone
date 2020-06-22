cmake_minimum_required(VERSION 3.7)

add_definitions(-std=c++17)

set(CXX_FLAGS "-Wall")
set(CMAKE_CXX_FLAGS, "${CXX_FLAGS}")

project(NetworkImpl)

add_executable(NetworkImpl src/main.cpp src/ip.cpp src/icmp.cpp src/networkinterface.cpp)

target_link_libraries(NetworkImpl /usr/local/lib/libviface.so)
