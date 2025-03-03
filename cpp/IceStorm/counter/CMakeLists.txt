cmake_minimum_required(VERSION 3.16)

project(icestorm_counter CXX)

include(../../cmake/common.cmake)

add_executable(client Client.cpp CounterObserverI.cpp CounterObserverI.h Counter.ice)
slice2cpp_generate(client)
target_link_libraries(client Ice::Ice Ice::IceStorm)
add_custom_command(TARGET client POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy -t $<TARGET_FILE_DIR:client> $<TARGET_RUNTIME_DLLS:client>
  COMMAND_EXPAND_LISTS
)

add_executable(server Server.cpp CounterI.cpp CounterI.h Counter.ice)
slice2cpp_generate(server)
target_link_libraries(server Ice::Ice Ice::IceStorm)
add_custom_command(TARGET server POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy -t $<TARGET_FILE_DIR:server> $<TARGET_RUNTIME_DLLS:server>
  COMMAND_EXPAND_LISTS
)
