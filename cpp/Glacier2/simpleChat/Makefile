# **********************************************************************
#
# Copyright (c) 2003-2015 ZeroC, Inc. All rights reserved.
#
# **********************************************************************

top_srcdir	= ../..

CLIENT		= client
SERVER		= server

TARGETS		= $(CLIENT) $(SERVER)

SLICE_OBJS	= Chat.o

COBJS		= $(SLICE_OBJS) \
		  Client.o

SOBJS		= $(SLICE_OBJS) \
		  ChatSessionI.o \
		  Server.o

OBJS		= $(COBJS) \
		  $(SOBJS)

include $(top_srcdir)/make/Make.rules

CPPFLAGS	:= -I. $(CPPFLAGS)

ifeq ($(BUILD_WITH_ICEE),yes)
    GLACIER2_LIBS = -Wl,-B$(BUILD_DEMOS)
endif
GLACIER2_LIBS	+= -lGlacier2

$(CLIENT): $(COBJS)
	rm -f $@
	$(CXX) $(LDFLAGS) $(LDEXEFLAGS) -o $@ $(COBJS) $(GLACIER2_LIBS) $(LIBS)
ifeq ($(BUILD_WITH_ICEE),yes)
	$(Q)$(STRIP) --strip-all --remove-section=.comment --remove-section=.note $(@)
endif

$(SERVER): $(SOBJS)
	rm -f $@
	$(CXX) $(LDFLAGS) $(LDEXEFLAGS) -o $@ $(SOBJS) $(GLACIER2_LIBS) $(LIBS)
ifeq ($(BUILD_WITH_ICEE),yes)
	$(Q)$(STRIP) --strip-all --remove-section=.comment --remove-section=.note $(@)
endif
