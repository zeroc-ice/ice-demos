cmake_minimum_required(VERSION 3.21)

project(mtalk CXX)

include(../../cmake/common.cmake)

add_executable(client Client.cpp MTalk.ice)
slice2cpp_generate(client)
target_link_libraries(client Ice::Ice)
add_custom_command(TARGET client POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy -t $<TARGET_FILE_DIR:client> $<TARGET_RUNTIME_DLLS:client>
  COMMAND_EXPAND_LISTS
)
