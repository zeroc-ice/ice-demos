# **********************************************************************
#
# Copyright (c) 2003-2016 ZeroC, Inc. All rights reserved.
#
# **********************************************************************

top_srcdir	= ../..

CLIENT		= client

TARGETS		= $(CLIENT)

SLICE_OBJS	= Hello.o \
		  Router.o

COBJS		= Client.o

OBJS		= $(SLICE_OBJS) $(COBJS)

include $(top_srcdir)/make/Make.rules

CPPFLAGS	:= -I. $(CPPFLAGS)

$(CLIENT): $(SLICE_OBJS) $(COBJS)
	rm -f $@
	$(CC) $(LDFLAGS) $(LDEXEFLAGS) -o $@ $(SLICE_OBJS) $(COBJS) -lGlacier2Objc $(LIBS)
