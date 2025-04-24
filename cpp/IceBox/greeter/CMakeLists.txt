cmake_minimum_required(VERSION 3.21)

project(icebox_greeter CXX)

include(../../cmake/common.cmake)

add_executable(client Client.cpp Greeter.ice)
slice2cpp_generate(client)
target_link_libraries(client Ice::Ice)
add_custom_command(TARGET client POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy -t $<TARGET_FILE_DIR:client>
    $<TARGET_RUNTIME_DLLS:client>
    $<GENEX_EVAL:$<TARGET_PROPERTY:Ice::Ice,ICE_RUNTIME_DLLS>>
  COMMAND_EXPAND_LISTS
)

add_library(GreeterService SHARED Chatbot.h Chatbot.cpp GreeterService.h GreeterService.cpp Greeter.ice)
slice2cpp_generate(GreeterService)
target_link_libraries(GreeterService PRIVATE Ice::Ice Ice::IceBox)
add_custom_command(TARGET GreeterService POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy -t $<TARGET_FILE_DIR:GreeterService>
    $<TARGET_RUNTIME_DLLS:GreeterService>
    $<GENEX_EVAL:$<TARGET_PROPERTY:Ice::Ice,ICE_RUNTIME_DLLS>>
  COMMAND_EXPAND_LISTS
)
