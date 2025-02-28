cmake_minimum_required(VERSION 3.21)

project(greeter CXX)

include(../../cmake/common.cmake)

add_executable(client Client.cpp Greeter.ice)
slice2cpp_generate(client)
target_link_libraries(client Ice::Ice)
add_custom_command(TARGET client POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy -t $<TARGET_FILE_DIR:client> $<TARGET_RUNTIME_DLLS:client>
  COMMAND_EXPAND_LISTS
)

add_executable(server Server.cpp Chatbot.cpp Chatbot.h Greeter.ice)
slice2cpp_generate(server)
target_link_libraries(server Ice::Ice)
add_custom_command(TARGET server POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy -t $<TARGET_FILE_DIR:server> $<TARGET_RUNTIME_DLLS:server>
  COMMAND_EXPAND_LISTS
)

add_executable(serveramd ServerAMD.cpp ChatbotAMD.cpp ChatbotAMD.h GreeterAMD.ice)
slice2cpp_generate(serveramd)
target_link_libraries(serveramd Ice::Ice)
add_custom_command(TARGET serveramd POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy -t $<TARGET_FILE_DIR:serveramd> $<TARGET_RUNTIME_DLLS:serveramd>
  COMMAND_EXPAND_LISTS
)
