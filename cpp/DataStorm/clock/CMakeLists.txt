cmake_minimum_required(VERSION 3.21)

project(datastorm_clock CXX)

include(../../cmake/common.cmake)

add_executable(reader Reader.cpp)
target_link_libraries(reader Ice::Ice Ice::DataStorm)
add_custom_command(TARGET reader POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy -t $<TARGET_FILE_DIR:reader>
    $<TARGET_RUNTIME_DLLS:reader>
    $<GENEX_EVAL:$<TARGET_PROPERTY:Ice::Ice,ICE_RUNTIME_DLLS>>
  COMMAND_EXPAND_LISTS
)

add_executable(writer Writer.cpp)
target_link_libraries(writer Ice::Ice Ice::DataStorm)
add_custom_command(TARGET writer POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy -t $<TARGET_FILE_DIR:writer>
    $<TARGET_RUNTIME_DLLS:writer>
    $<GENEX_EVAL:$<TARGET_PROPERTY:Ice::Ice,ICE_RUNTIME_DLLS>>
  COMMAND_EXPAND_LISTS
)
