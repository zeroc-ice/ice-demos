cmake_minimum_required(VERSION 3.21)

project(icegrid_icebox CXX)

include(../../cmake/common.cmake)
include(../../cmake/IceExe.cmake) # temporary

add_executable(client Client.cpp Greeter.ice)
slice2cpp_generate(client)
target_link_libraries(client PRIVATE Ice::Ice)
add_custom_command(TARGET client POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy -t $<TARGET_FILE_DIR:client>
    $<TARGET_RUNTIME_DLLS:client>
    $<GENEX_EVAL:$<TARGET_PROPERTY:Ice::Ice,ICE_RUNTIME_DLLS>>
  COMMAND_EXPAND_LISTS
)

add_library(GreeterService SHARED Chatbot.h Chatbot.cpp GreeterService.h GreeterService.cpp Greeter.ice)
if(WIN32)
  set_target_properties(GreeterService PROPERTIES DEBUG_POSTFIX "d")
endif()
slice2cpp_generate(GreeterService)
target_link_libraries(GreeterService PRIVATE Ice::Ice Ice::IceBox)
if(WIN32)
  add_custom_command(TARGET GreeterService POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy -t $<TARGET_FILE_DIR:GreeterService>
      $<TARGET_RUNTIME_DLLS:GreeterService>
      $<GENEX_EVAL:$<TARGET_PROPERTY:Ice::Ice,ICE_RUNTIME_DLLS>>
    COMMAND_EXPAND_LISTS
  )
  add_custom_command(TARGET GreeterService POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy -t $<TARGET_FILE_DIR:GreeterService>
      $<TARGET_FILE:Ice::icebox_EXE>
  )
endif()
