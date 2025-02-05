cmake_minimum_required(VERSION 3.16)

project(clock CXX)

include(../../cmake/common.cmake)

add_executable(publisher Publisher.cpp Clock.ice)
slice2cpp_generate(publisher)
target_link_libraries(publisher Ice::Ice)

add_executable(subscriber Subscriber.cpp Clock.ice)
slice2cpp_generate(subscriber)
target_link_libraries(subscriber Ice::Ice)
