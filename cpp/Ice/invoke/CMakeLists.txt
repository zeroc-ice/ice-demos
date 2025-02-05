cmake_minimum_required(VERSION 3.16)

project(invoke CXX)

include(../../cmake/common.cmake)

add_executable(client Client.cpp Printer.ice)
slice2cpp_generate(client)
target_link_libraries(client Ice::Ice)

add_executable(server Server.cpp PrinterI.cpp PrinterI.h Printer.ice)
slice2cpp_generate(server)
target_link_libraries(server Ice::Ice)
