# **********************************************************************
#
# Copyright (c) 2003-2015 ZeroC, Inc. All rights reserved.
#
# This copy of Chat Demo is licensed to you under the terms described
# in the CHAT_DEMO_LICENSE file included in this distribution.
#
# **********************************************************************

top_srcdir     = ../..

GL2CLIENT	= chatgl2client
POLLCLIENT	= chatpollclient

TARGETS		= $(GL2CLIENT) $(POLLCLIENT)

SLICE_OBJS 	= Chat.o \
		  ChatSession.o \
		  PollingChat.o

GL2_CLIENT_OBJS	= Chat.o \
		  ChatSession.o \
		  ChatUtils.o \
		  Client.o

POLL_CLIENT_OBJS = Chat.o \
		   PollingChat.o \
		   ChatUtils.o \
		   PollingClient.o

SRCS		= $(GL2_CLIENT_OBJS:.o=.cpp) \
		  $(POLL_CLIENT_OBJS:.o=.cpp)

include $(top_srcdir)/make/Make.rules

CPPFLAGS	:= -I. $(CPPFLAGS)
SLICE2CPPFLAGS  := -I. $(SLICE2CPPFLAGS)

$(GL2CLIENT): $(GL2_CLIENT_OBJS)
	rm -f $@
	$(CXX) $(LDFLAGS) -o $@ $(GL2_CLIENT_OBJS) -lGlacier2 $(LIBS)

$(POLLCLIENT): $(POLL_CLIENT_OBJS)
	rm -f $@
	$(CXX) $(LDFLAGS) -o $@ $(POLL_CLIENT_OBJS) $(LIBS)
