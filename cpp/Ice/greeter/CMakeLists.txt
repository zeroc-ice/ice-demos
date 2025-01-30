cmake_minimum_required(VERSION 3.15)
project(IceGreeter LANGUAGES CXX)

if (WIN32 OR APPLE)
    set(CMAKE_CXX_STANDARD 20)
else()
    set(CMAKE_CXX_STANDARD 17)
endif()
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(../../cmake/Ice.cmake)

add_executable(client Client.cpp)
slice2cpp(client Greeter.ice)
target_include_directories(client PRIVATE Ice::Ice)
target_link_libraries(client Ice::Ice)

add_executable(server Server.cpp Chatbot.cpp Chatbot.h)
slice2cpp(server Greeter.ice)
target_include_directories(server PRIVATE Ice::Ice)
target_link_libraries(server Ice::Ice)
