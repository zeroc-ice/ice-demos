cmake_minimum_required(VERSION 3.16)

project(session CXX)

include(../../cmake/common.cmake)

add_executable(client Client.cpp Session.ice)
slice2cpp_generate(client)
target_link_libraries(client Ice::Ice)

add_executable(server Server.cpp SessionFactoryI.cpp SessionFactoryI.h SessionI.cpp SessionI.h Session.ice)
slice2cpp_generate(server)
target_link_libraries(server Ice::Ice)
