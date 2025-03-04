cmake_minimum_required(VERSION 3.21)

project(multicast CXX)

include(../../cmake/common.cmake)

add_executable(client Client.cpp Hello.ice Discovery.ice)
slice2cpp_generate(client)
target_link_libraries(client Ice::Ice)

add_executable(server Server.cpp Hello.ice Discovery.ice)
slice2cpp_generate(server)
target_link_libraries(server Ice::Ice)
