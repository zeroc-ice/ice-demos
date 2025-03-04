cmake_minimum_required(VERSION 3.21)

project(datastorm_stock CXX)

include(../../cmake/common.cmake)

# This demo requires C++20
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(reader Reader.cpp Stock.ice)
slice2cpp_generate(reader)
target_link_libraries(reader Ice::Ice Ice::DataStorm)

add_executable(writer Writer.cpp Stock.ice)
slice2cpp_generate(writer)
target_link_libraries(writer Ice::Ice Ice::DataStorm)
