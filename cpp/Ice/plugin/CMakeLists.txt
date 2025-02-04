cmake_minimum_required(VERSION 3.16)

project(plugin CXX)

include(../../cmake/common.cmake)

add_executable(client Client.cpp Hello.ice)
slice2cpp_generate(client)
target_link_libraries(client Ice::Ice)

add_executable(server Server.cpp Hello.ice)
slice2cpp_generate(server)
target_link_libraries(server Ice::Ice)

add_library(LoggerPlugin SHARED LoggerPluginI.cpp)
target_link_libraries(LoggerPlugin Ice::Ice)

add_library(HelloPlugin SHARED HelloPluginI.cpp)
slice2cpp_generate(HelloPlugin)
target_link_libraries(HelloPlugin Ice::Ice)
