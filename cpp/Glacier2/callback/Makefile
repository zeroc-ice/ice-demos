# **********************************************************************
#
# Copyright (c) 2003-2017 ZeroC, Inc. All rights reserved.
#
# **********************************************************************

top_srcdir	= ../..

CLIENT		= client
SERVER		= server

TARGETS		= $(CLIENT) $(SERVER) $(SESSION_SERVER)

SLICE_OBJS	= Callback.o

COBJS		= $(SLICE_OBJS) \
		  CallbackI.o \
		  Client.o

SOBJS		= $(SLICE_OBJS) \
		  CallbackI.o \
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
ifeq ($(BUILD_DEMOS),static)
	$(Q)$(STRIP) --strip-all --remove-section=.comment --remove-section=.note $(@)
endif

$(SERVER): $(SOBJS)
	rm -f $@
	$(CXX) $(LDFLAGS) $(LDEXEFLAGS) -o $@ $(SOBJS) $(LIBS)
ifeq ($(BUILD_DEMOS),static)
	$(Q)$(STRIP) --strip-all --remove-section=.comment --remove-section=.note $(@)
endif
