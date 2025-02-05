cmake_minimum_required(VERSION 3.16)

project(mtalk CXX)

include(../../cmake/common.cmake)

add_executable(client Client.cpp MTalk.ice)
slice2cpp_generate(client)
target_link_libraries(client Ice::Ice)
