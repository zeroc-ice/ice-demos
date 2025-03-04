cmake_minimum_required(VERSION 3.21)

project(icegrid_custom_locator CXX)

include(../../cmake/common.cmake)

add_executable(client Client.cpp Hello.ice)
slice2cpp_generate(client)
target_link_libraries(client Ice::Ice)

add_executable(server Server.cpp HelloI.cpp HelloI.h Hello.ice)
slice2cpp_generate(server)
target_link_libraries(server Ice::Ice)

add_executable(locator Locator.cpp)
target_link_libraries(locator Ice::Ice)
