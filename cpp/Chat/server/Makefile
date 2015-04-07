# **********************************************************************
#
# Copyright (c) 2003-2015 ZeroC, Inc. All rights reserved.
#
# This copy of Chat Demo is licensed to you under the terms described
# in the CHAT_DEMO_LICENSE file included in this distribution.
#
# **********************************************************************

top_srcdir     	= ../..

SERVER 		= chatserver

TARGETS		= $(SERVER)

SLICE_OBJS	= Chat.o \
		  ChatSession.o \
		  PollingChat.o

OBJS		= $(SLICE_OBJS) \
		  ChatUtils.o \
		  ChatSessionI.o \
		  ChatSessionManagerI.o \
		  PollingChatSessionI.o \
		  PollingChatSessionFactoryI.o \
		  ChatRoom.o \
		  ChatServer.o

include $(top_srcdir)/make/Make.rules

CPPFLAGS	:= -I. $(CPPFLAGS)
SLICE2CPPFLAGS  := -I. $(SLICE2CPPFLAGS)

$(SERVER): $(OBJS)
	rm -f $@
	$(CXX) $(LDFLAGS) -o $@ $(OBJS) -lGlacier2 $(LIBS)
