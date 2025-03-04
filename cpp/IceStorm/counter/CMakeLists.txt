cmake_minimum_required(VERSION 3.21)

project(icestorm_counter CXX)

include(../../cmake/common.cmake)

add_executable(client Client.cpp CounterObserverI.cpp CounterObserverI.h Counter.ice)
slice2cpp_generate(client)
target_link_libraries(client Ice::Ice Ice::IceStorm)

add_executable(server Server.cpp CounterI.cpp CounterI.h Counter.ice)
slice2cpp_generate(server)
target_link_libraries(server Ice::Ice Ice::IceStorm)
