cmake_minimum_required(VERSION 3.16)

project(interleaved CXX)

include(../../cmake/common.cmake)

add_executable(client Client.cpp Throughput.ice)
slice2cpp_generate(client)
target_link_libraries(client Ice::Ice)

add_executable(server Server.cpp ThroughputI.cpp ThroughputI.h Throughput.ice)
slice2cpp_generate(server)
target_link_libraries(server Ice::Ice)
