cmake_minimum_required(VERSION 3.16)

project(glacier2_simpleChat CXX)

include(../../cmake/common.cmake)

add_executable(client Client.cpp Chat.ice)
slice2cpp_generate(client)
target_link_libraries(client Ice::Ice Ice::Glacier2)

add_executable(server Server.cpp ChatSessionI.cpp ChatSessionI.h Chat.ice)
slice2cpp_generate(server)
target_link_libraries(server Ice::Ice Ice::Glacier2)
