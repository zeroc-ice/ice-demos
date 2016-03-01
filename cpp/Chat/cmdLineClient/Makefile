# **********************************************************************
#
# Copyright (c) 2003-2016 ZeroC, Inc. All rights reserved.
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

ifeq ($(BUILD_WITH_ICEE),yes)
    GLACIER2_LIBS = -Wl,-B$(BUILD_DEMOS)
endif
GLACIER2_LIBS	+= -lGlacier2

$(GL2CLIENT): $(GL2_CLIENT_OBJS)
	rm -f $@
	$(CXX) $(LDFLAGS) $(LDEXEFLAGS) -o $@ $(GL2_CLIENT_OBJS) $(GLACIER2_LIBS) $(LIBS)
ifeq ($(BUILD_WITH_ICEE),yes)
	$(Q)$(STRIP) --strip-all --remove-section=.comment --remove-section=.note $(@)
endif

$(POLLCLIENT): $(POLL_CLIENT_OBJS)
	rm -f $@
	$(CXX) $(LDFLAGS) $(LDEXEFLAGS) -o $@ $(POLL_CLIENT_OBJS) $(LIBS)
ifeq ($(BUILD_WITH_ICEE),yes)
	$(Q)$(STRIP) --strip-all --remove-section=.comment --remove-section=.note $(@)
endif
