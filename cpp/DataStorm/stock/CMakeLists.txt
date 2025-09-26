cmake_minimum_required(VERSION 3.21)

project(datastorm_stock CXX)

include(../../cmake/common.cmake)

# This demo requires C++20
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(reader Reader.cpp Stock.ice)
slice2cpp_generate(reader)
target_link_libraries(reader PRIVATE Ice::Ice Ice::DataStorm)
if(WIN32)
  add_custom_command(TARGET reader POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy -t $<TARGET_FILE_DIR:reader>
      $<TARGET_RUNTIME_DLLS:reader>
      $<GENEX_EVAL:$<TARGET_PROPERTY:Ice::Ice,ICE_RUNTIME_DLLS>>
    COMMAND_EXPAND_LISTS
  )
endif()

add_executable(writer Writer.cpp Stock.ice)
slice2cpp_generate(writer)
target_link_libraries(writer PRIVATE Ice::Ice Ice::DataStorm)
if(WIN32)
  add_custom_command(TARGET writer POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy -t $<TARGET_FILE_DIR:writer>
      $<TARGET_RUNTIME_DLLS:writer>
      $<GENEX_EVAL:$<TARGET_PROPERTY:Ice::Ice,ICE_RUNTIME_DLLS>>
    COMMAND_EXPAND_LISTS
  )
endif()
