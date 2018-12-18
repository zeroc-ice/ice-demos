# **********************************************************************
#
# Copyright (c) 2003-present ZeroC, Inc. All rights reserved.
#
# **********************************************************************

$(demo)_programs        = chatserver
$(demo)_dependencies    = Ice Glacier2

$(demo)_chatserver_sources      = Chat.ice \
                                  ChatSession.ice \
                                  PollingChat.ice \
                                  ChatUtils.cpp \
                                  ChatSessionI.cpp \
                                  ChatSessionManagerI.cpp \
                                  PollingChatSessionI.cpp \
                                  PollingChatSessionFactoryI.cpp \
                                  ChatRoom.cpp \
                                  ChatServer.cpp

demos += $(demo)
