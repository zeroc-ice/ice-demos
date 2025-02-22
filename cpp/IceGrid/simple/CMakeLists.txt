cmake_minimum_required(VERSION 3.16)

project(icegrid_simple CXX)

include(../../cmake/common.cmake)

add_executable(client Client.cpp Hello.ice)
slice2cpp_generate(client)
target_link_libraries(client Ice::Ice Ice::IceGrid Ice::Glacier2)

add_executable(server Server.cpp HelloI.cpp HelloI.h Hello.ice)
slice2cpp_generate(server)
target_link_libraries(server Ice::Ice)
