cmake_minimum_required(VERSION 3.16)

project(chat_client CXX)

include(../../cmake/common.cmake)

add_executable(chatserver
    ChatRoom.cpp ChatRoom.h
    ChatServer.cpp
    ChatSessionI.cpp ChatSessionI.h
    ChatSessionManagerI.cpp ChatSessionManagerI.h
    Chat.ice ChatSession.ice

    PollingChatSessionFactoryI.cpp PollingChatSessionFactoryI.h
    PollingChatSessionI.cpp PollingChatSessionI.h
    PollingChat.ice

    ChatUtils.cpp ChatUtils.h
)
slice2cpp_generate(chatserver)
target_link_libraries(chatserver Ice::Ice Ice::Glacier2)
