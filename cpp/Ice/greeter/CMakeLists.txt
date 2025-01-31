cmake_minimum_required(VERSION 3.15)
project(IceGreeter LANGUAGES CXX)

if (WIN32 OR APPLE)
    set(CMAKE_CXX_STANDARD 20)
else()
    set(CMAKE_CXX_STANDARD 17)
endif()
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})

include(../../cmake/Ice.cmake)

add_executable(ice_greeter_client Client.cpp)
set_property(TARGET ice_greeter_client PROPERTY OUTPUT_NAME client)
slice2cpp(ice_greeter_client Greeter.ice)
target_include_directories(ice_greeter_client PRIVATE Ice::Ice)
target_link_libraries(ice_greeter_client Ice::Ice)

add_executable(ice_greeter_server Server.cpp Chatbot.cpp Chatbot.h)
set_property(TARGET ice_greeter_server PROPERTY OUTPUT_NAME server)
slice2cpp(ice_greeter_server Greeter.ice)
target_include_directories(ice_greeter_server PRIVATE Ice::Ice)
target_link_libraries(ice_greeter_server Ice::Ice)
