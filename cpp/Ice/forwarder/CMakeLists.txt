cmake_minimum_required(VERSION 3.21)

project(greeter CXX)

include(../../cmake/common.cmake)

add_executable(client Client.cpp Greeter.ice)
slice2cpp_generate(client)
target_link_libraries(client Ice::Ice)

add_executable(server Server.cpp Chatbot.cpp Chatbot.h Greeter.ice)
slice2cpp_generate(server)
target_link_libraries(server Ice::Ice)

add_executable(forwardingserver Forwarder.cpp Forwarder.h ForwardingServer.cpp)
target_link_libraries(forwardingserver Ice::Ice)
