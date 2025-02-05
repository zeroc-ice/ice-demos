cmake_minimum_required(VERSION 3.16)

project(nested CXX)

include(../../cmake/common.cmake)

add_executable(client Client.cpp NestedI.cpp NestedI.h Nested.ice)
slice2cpp_generate(client)
target_link_libraries(client Ice::Ice)

add_executable(server Server.cpp NestedI.cpp NestedI.h Nested.ice)
slice2cpp_generate(server)
target_link_libraries(server Ice::Ice)
