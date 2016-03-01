# **********************************************************************
#
# Copyright (c) 2003-2016 ZeroC, Inc. All rights reserved.
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

ifeq ($(BUILD_WITH_ICEE),yes)
    GLACIER2_LIBS = -Wl,-B$(BUILD_DEMOS)
endif
GLACIER2_LIBS	+= -lGlacier2

$(SERVER): $(OBJS)
	rm -f $@
	$(CXX) $(LDFLAGS) $(LDEXEFLAGS) -o $@ $(OBJS) $(GLACIER2_LIBS) $(LIBS)
ifeq ($(BUILD_WITH_ICEE),yes)
	$(Q)$(STRIP) --strip-all --remove-section=.comment --remove-section=.note $(@)
endif
