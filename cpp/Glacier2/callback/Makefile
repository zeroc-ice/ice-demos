# **********************************************************************
#
# Copyright (c) 2003-2015 ZeroC, Inc. All rights reserved.
#
# **********************************************************************

top_srcdir	= ../..

MAXWARN         = yes

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

$(CLIENT): $(COBJS)
	rm -f $@
	$(CXX) $(LDFLAGS) $(LDEXEFLAGS) -o $@ $(COBJS) -lGlacier2 $(LIBS)

$(SERVER): $(SOBJS)
	rm -f $@
	$(CXX) $(LDFLAGS) $(LDEXEFLAGS) -o $@ $(SOBJS) $(LIBS)
