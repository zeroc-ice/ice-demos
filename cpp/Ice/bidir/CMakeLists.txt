cmake_minimum_required(VERSION 3.16)

project(bidir CXX)

include(../../cmake/common.cmake)

add_executable(client Client.cpp Callback.ice)
slice2cpp_generate(client)
target_link_libraries(client Ice::Ice)

add_executable(server Server.cpp CallbackI.cpp CallbackI.h Callback.ice)
slice2cpp_generate(server)
target_link_libraries(server Ice::Ice)
