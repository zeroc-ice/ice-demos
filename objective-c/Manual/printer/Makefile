# **********************************************************************
#
# Copyright (c) 2003-2017 ZeroC, Inc. All rights reserved.
#
# **********************************************************************

top_srcdir	= ../..

CLIENT		= client
SERVER		= server

TARGETS		= $(CLIENT) $(SERVER)

SLICE_OBJS	= Printer.o

COBJS		= Client.o

SOBJS		= Server.o

OBJS		= $(SLICE_OBJS) $(COBJS) $(SOBJS)

include $(top_srcdir)/make/Make.rules

CPPFLAGS	:= -I. $(CPPFLAGS)

$(CLIENT): $(SLICE_OBJS) $(COBJS)
	rm -f $@
	$(CC) $(LDFLAGS) $(LDEXEFLAGS) -o $@ $(SLICE_OBJS) $(COBJS) $(LIBS)

$(SERVER): $(SLICE_OBJS) $(SOBJS)
	rm -f $@
	$(CC) $(LDFLAGS) $(LDEXEFLAGS) -o $@ $(SLICE_OBJS) $(SOBJS) $(LIBS)
