# **********************************************************************
#
# Copyright (c) 2003-2015 ZeroC, Inc. All rights reserved.
#
# This copy of Ice is licensed to you under the terms described in the
# ICE_LICENSE file included in this distribution.
#
# **********************************************************************

$(demo)_programs	= chatserver
$(demo)_dependencies	= Ice Glacier2

$(demo)_chatserver_sources	= Chat.ice \
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
