cmake_minimum_required(VERSION 3.21)

project(datastorm_clock CXX)

include(../../cmake/common.cmake)

add_executable(reader Reader.cpp)
target_link_libraries(reader Ice::Ice Ice::DataStorm)

add_executable(writer Writer.cpp)
target_link_libraries(writer Ice::Ice Ice::DataStorm)
