cmake_minimum_required(VERSION 3.16)

project(chat_client CXX)

include(../../cmake/common.cmake)

set(common_sources ChatUtils.cpp ChatUtils.h Chat.ice)

add_executable(chatgl2client Client.cpp ChatSession.ice ${common_sources})
slice2cpp_generate(chatgl2client)
target_link_libraries(chatgl2client Ice::Ice Ice::Glacier2)

add_executable(chatpollclient PollingClient.cpp PollingChat.ice ${common_sources})
slice2cpp_generate(chatpollclient)
target_link_libraries(chatpollclient Ice::Ice)
