cmake_minimum_required(VERSION 3.16)

project(context CXX)

include(../../cmake/common.cmake)

add_executable(client Client.cpp Greeter.ice)
slice2cpp_generate(client)
target_link_libraries(client Ice::Ice)

add_executable(server
    Server.cpp
    AuthorizationMiddleware.cpp AuthorizationMiddleware.h
    Chatbot.cpp Chatbot.h
    Greeter.ice)
slice2cpp_generate(server)
target_link_libraries(server Ice::Ice)
