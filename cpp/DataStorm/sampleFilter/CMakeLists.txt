cmake_minimum_required(VERSION 3.16)

project(datastorm_sampleFilter CXX)

include(../../cmake/common.cmake)

add_executable(reader Reader.cpp)
target_link_libraries(reader Ice::Ice Ice::DataStorm)

add_executable(writer Writer.cpp)
target_link_libraries(writer Ice::Ice Ice::DataStorm)
