cmake_minimum_required(VERSION 3.21)

project(optional CXX)

include(../../cmake/common.cmake)

add_executable(client1 Client1.cpp WeatherStation1.ice)
slice2cpp_generate(client1)
target_link_libraries(client1 Ice::Ice)
add_custom_command(TARGET client1 POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy -t $<TARGET_FILE_DIR:client1>
    $<TARGET_RUNTIME_DLLS:client1>
    $<GENEX_EVAL:$<TARGET_PROPERTY:Ice::Ice,ICE_RUNTIME_DLLS>>
  COMMAND_EXPAND_LISTS
)

add_executable(client2 Client2.cpp WeatherStation2.ice)
slice2cpp_generate(client2)
target_link_libraries(client2 Ice::Ice)
add_custom_command(TARGET client2 POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy -t $<TARGET_FILE_DIR:client2>
    $<TARGET_RUNTIME_DLLS:client2>
    $<GENEX_EVAL:$<TARGET_PROPERTY:Ice::Ice,ICE_RUNTIME_DLLS>>
  COMMAND_EXPAND_LISTS
)

add_executable(server1 Server1.cpp ConsolePrinter1.cpp ConsolePrinter1.h WeatherStation1.ice)
slice2cpp_generate(server1)
target_link_libraries(server1 Ice::Ice)
add_custom_command(TARGET server1 POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy -t $<TARGET_FILE_DIR:server1>
    $<TARGET_RUNTIME_DLLS:server1>
    $<GENEX_EVAL:$<TARGET_PROPERTY:Ice::Ice,ICE_RUNTIME_DLLS>>
  COMMAND_EXPAND_LISTS
)

add_executable(server2 Server2.cpp ConsolePrinter2.cpp ConsolePrinter2.h WeatherStation2.ice)
slice2cpp_generate(server2)
target_link_libraries(server2 Ice::Ice)
add_custom_command(TARGET server2 POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy -t $<TARGET_FILE_DIR:server2>
    $<TARGET_RUNTIME_DLLS:server2>
    $<GENEX_EVAL:$<TARGET_PROPERTY:Ice::Ice,ICE_RUNTIME_DLLS>>
  COMMAND_EXPAND_LISTS
)
