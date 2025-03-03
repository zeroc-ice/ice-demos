cmake_minimum_required(VERSION 3.16)

project(glacier2_callback CXX)

include(../../cmake/common.cmake)

add_executable(client Client.cpp Callback.ice)
slice2cpp_generate(client)
target_link_libraries(client Ice::Ice Ice::Glacier2)
add_custom_command(TARGET client POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy -t $<TARGET_FILE_DIR:client> $<TARGET_RUNTIME_DLLS:client>
  COMMAND_EXPAND_LISTS
)

add_executable(server Server.cpp CallbackI.cpp CallbackI.h Callback.ice)
slice2cpp_generate(server)
target_link_libraries(server Ice::Ice)
add_custom_command(TARGET server POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy -t $<TARGET_FILE_DIR:server> $<TARGET_RUNTIME_DLLS:server>
  COMMAND_EXPAND_LISTS
)
