cmake_minimum_required(VERSION 3.21)

project(icebox_hello CXX)

include(../../cmake/common.cmake)

add_executable(client Client.cpp Hello.ice)
slice2cpp_generate(client)
target_link_libraries(client Ice::Ice)
add_custom_command(TARGET client POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy -t $<TARGET_FILE_DIR:client> $<TARGET_RUNTIME_DLLS:client>
  COMMAND_EXPAND_LISTS
)

add_library(HelloService SHARED HelloI.cpp HelloI.h HelloServiceI.cpp HelloServiceI.h Hello.ice)
slice2cpp_generate(HelloService)
target_link_libraries(HelloService Ice::Ice Ice::IceBox)
add_custom_command(TARGET HelloService POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy -t $<TARGET_FILE_DIR:HelloService> $<TARGET_RUNTIME_DLLS:HelloService>
  COMMAND_EXPAND_LISTS
)
