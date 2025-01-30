cmake_minimum_required(VERSION 3.15)
project(IceGreeter LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/bin)

include(../../cmake/Ice.cmake)

add_executable(ice_greeter_async_client Client.cpp)
set_property(TARGET ice_greeter_async_client PROPERTY OUTPUT_NAME client)
slice2cpp(ice_greeter_async_client Greeter.ice)
target_include_directories(ice_greeter_async_client PRIVATE Ice::Ice)
target_link_libraries(ice_greeter_async_client Ice::Ice)

add_executable(ice_greeter_async_server Server.cpp Chatbot.cpp Chatbot.h)
set_property(TARGET ice_greeter_async_server PROPERTY OUTPUT_NAME server)
slice2cpp(ice_greeter_async_server Greeter.ice)
target_include_directories(ice_greeter_async_server PRIVATE Ice::Ice)
target_link_libraries(ice_greeter_async_server Ice::Ice)
