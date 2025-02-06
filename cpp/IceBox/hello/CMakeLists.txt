cmake_minimum_required(VERSION 3.16)

project(icebox_hello CXX)

include(../../cmake/common.cmake)

add_executable(client Client.cpp Hello.ice)
slice2cpp_generate(client)
target_link_libraries(client Ice::Ice)

add_library(HelloService SHARED HelloI.cpp HelloI.h HelloServiceI.cpp HelloServiceI.h Hello.ice)
slice2cpp_generate(HelloService)
target_link_libraries(HelloService Ice::Ice Ice::IceBox)
