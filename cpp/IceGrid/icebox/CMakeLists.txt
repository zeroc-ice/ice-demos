cmake_minimum_required(VERSION 3.16)

project(icegrid_icebox CXX)

include(../../cmake/common.cmake)

add_executable(client Client.cpp Hello.ice)
slice2cpp_generate(client)
target_link_libraries(client Ice::Ice Ice::IceGrid Ice::Glacier2)

add_library(HelloService SHARED HelloServiceI.cpp HelloServiceI.h HelloI.cpp HelloI.h Hello.ice)
slice2cpp_generate(HelloService)
target_link_libraries(HelloService Ice::Ice Ice::IceBox)
