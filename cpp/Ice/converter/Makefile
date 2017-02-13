# **********************************************************************
#
# Copyright (c) 2003-2017 ZeroC, Inc. All rights reserved.
#
# **********************************************************************

top_srcdir	= ../..

CLIENT1		= client1
CLIENT2		= client2
SERVER		= server

TARGETS		= $(CLIENT1) $(CLIENT2) $(SERVER)

SLICE_OBJS	= Greet.o

C1OBJS		= $(SLICE_OBJS) \
		  Client.o \
		  ClientWithConverter.o \
		  StringConverterI.o

C2OBJS		= $(SLICE_OBJS) \
		  Client.o \
		  ClientWithoutConverter.o

SOBJS		= $(SLICE_OBJS) \
		  GreetI.o \
		  Server.o

OBJS		= $(C1OBJS) \
		  $(C2OBJS) \
		  $(SOBJS)

include $(top_srcdir)/make/Make.rules

CPPFLAGS	:= -I. $(CPPFLAGS)

$(CLIENT1): $(C1OBJS)
	rm -f $@
	$(CXX) $(LDFLAGS) $(LDEXEFLAGS) -o $@ $(C1OBJS) $(LIBS)
ifeq ($(BUILD_DEMOS),static)
	$(Q)$(STRIP) --strip-all --remove-section=.comment --remove-section=.note $(@)
endif

$(CLIENT2): $(C2OBJS)
	rm -f $@
	$(CXX) $(LDFLAGS) $(LDEXEFLAGS) -o $@ $(C2OBJS) $(LIBS)
ifeq ($(BUILD_DEMOS),static)
	$(Q)$(STRIP) --strip-all --remove-section=.comment --remove-section=.note $(@)
endif

$(SERVER): $(SOBJS)
	rm -f $@
	$(CXX) $(LDFLAGS) $(LDEXEFLAGS) -o $@ $(SOBJS) $(LIBS)
ifeq ($(BUILD_DEMOS),static)
	$(Q)$(STRIP) --strip-all --remove-section=.comment --remove-section=.note $(@)
endif
