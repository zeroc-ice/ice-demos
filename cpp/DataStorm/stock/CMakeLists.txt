cmake_minimum_required(VERSION 3.16)

project(datastorm_stock CXX)

include(../../cmake/common.cmake)

add_executable(reader Reader.cpp Stock.ice)
slice2cpp_generate(reader)
target_link_libraries(reader Ice::Ice Ice::DataStorm)

add_executable(writer Writer.cpp Stock.ice)
slice2cpp_generate(writer)
target_link_libraries(writer Ice::Ice Ice::DataStorm)
