cmake_minimum_required(VERSION 3.21)

project(icebox_greeter CXX)

include(../../cmake/common.cmake)

add_executable(client Client.cpp Greeter.ice)
slice2cpp_generate(client)
target_link_libraries(client Ice::Ice)

add_library(GreeterService SHARED Chatbot.h Chatbot.cpp GreeterService.h GreeterService.cpp Greeter.ice)
slice2cpp_generate(GreeterService)
target_link_libraries(GreeterService Ice::Ice Ice::IceBox)
