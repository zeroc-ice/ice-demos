cmake_minimum_required(VERSION 3.21)

project(filesystem CXX)

include(../../cmake/common.cmake)

add_executable(client Client.cpp Filesystem.ice)
slice2cpp_generate(client)
target_link_libraries(client PRIVATE Ice::Ice)
add_custom_command(TARGET client POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy -t $<TARGET_FILE_DIR:client>
    $<TARGET_RUNTIME_DLLS:client>
    $<GENEX_EVAL:$<TARGET_PROPERTY:Ice::Ice,ICE_RUNTIME_DLLS>>
  COMMAND_EXPAND_LISTS
)

add_executable(server Server.cpp Memory.cpp Memory.h Filesystem.ice)
slice2cpp_generate(server)
target_link_libraries(server PRIVATE Ice::Ice)
add_custom_command(TARGET server POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy -t $<TARGET_FILE_DIR:server>
    $<TARGET_RUNTIME_DLLS:server>
    $<GENEX_EVAL:$<TARGET_PROPERTY:Ice::Ice,ICE_RUNTIME_DLLS>>
  COMMAND_EXPAND_LISTS
)
