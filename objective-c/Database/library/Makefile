# **********************************************************************
#
# Copyright (c) 2003-2017 ZeroC, Inc. All rights reserved.
#
# **********************************************************************

top_srcdir	= ../..

CLIENT		= client

TARGETS		= $(CLIENT)

SLICE_OBJS 	= Library.o \
		  Session.o \
		  Glacier2Session.o

COBJS		= Client.o \
		  Grammar.o \
		  Scanner.o \
		  Parser.o \
	  	  RunParser.o

OBJS		= $(SLICE_OBJS) $(COBJS)

include $(top_srcdir)/make/Make.rules

CPPFLAGS	:= -I. $(CPPFLAGS)

#
# Flex and bison generated files doesn't support ARC.
#
Grammar.o: Grammar.m
	$(CC) -c $(CPPFLAGS) $(OBJCFLAGS) $(CFLAGS) $<

Scanner.o: Scanner.m
	$(CC) -c $(CPPFLAGS) $(OBJCFLAGS) $(CFLAGS) $<

$(CLIENT): $(SLICE_OBJS) $(COBJS)
	rm -f $@
	$(CC) $(LDFLAGS) -o $@ $(SLICE_OBJS) $(COBJS) -lGlacier2ObjC $(LIBS)
