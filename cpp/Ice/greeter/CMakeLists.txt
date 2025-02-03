cmake_minimum_required(VERSION 3.16)

project(IceGreeter CXX)

include(../../cmake/common.cmake)

add_executable(ice_greeter_client Client.cpp Greeter.ice)
slice2cpp_generate(ice_greeter_client)
set_target_properties(ice_greeter_client PROPERTIES OUTPUT_NAME client)
target_link_libraries(ice_greeter_client Ice::Ice)

add_executable(ice_greeter_server Server.cpp ChatBot.cpp ChatBot.h Greeter.ice)
slice2cpp_generate(ice_greeter_server)
set_target_properties(ice_greeter_server PROPERTIES OUTPUT_NAME server)
target_link_libraries(ice_greeter_server Ice::Ice)
