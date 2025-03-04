cmake_minimum_required(VERSION 3.21)

project(glacier2_callback CXX)

include(../../cmake/common.cmake)

add_executable(client Client.cpp Callback.ice)
slice2cpp_generate(client)
target_link_libraries(client Ice::Ice Ice::Glacier2)

add_executable(server Server.cpp CallbackI.cpp CallbackI.h Callback.ice)
slice2cpp_generate(server)
target_link_libraries(server Ice::Ice)
