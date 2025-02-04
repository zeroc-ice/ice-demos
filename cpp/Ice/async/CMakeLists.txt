cmake_minimum_required(VERSION 3.16)

project(async CXX)

include(../../cmake/common.cmake)

add_executable(client Client.cpp Hello.ice)
slice2cpp_generate(client)
target_link_libraries(client Ice::Ice)

add_executable(server Server.cpp WorkQueue.cpp WorkQueue.h Hello.ice HelloI.cpp HelloI.h)
slice2cpp_generate(server)
target_link_libraries(server Ice::Ice)
