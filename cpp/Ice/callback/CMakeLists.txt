cmake_minimum_required(VERSION 3.16)

project(callback CXX)

include(../../cmake/common.cmake)

add_executable(client Client.cpp MockAlarmClock.h MockAlarmClock.cpp AlarmClock.ice)
slice2cpp_generate(client)
target_link_libraries(client Ice::Ice)

add_executable(server Server.cpp SimpleWakeUpService.h SimpleWakeUpService.cpp AlarmClock.ice)
slice2cpp_generate(server)
target_link_libraries(server Ice::Ice)
