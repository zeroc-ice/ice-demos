# **********************************************************************
#
# Copyright (c) 2003-2017 ZeroC, Inc. All rights reserved.
#
# **********************************************************************

top_srcdir	= ../..

CLIENT		= client
SERVER		= server

TARGETS		= $(CLIENT) $(SERVER)

SLICE_OBJS	= Filesystem.o PersistentFilesystem.o

COBJS		= Filesystem.o \
		  Client.o \
		  Grammar.o \
		  Parser.o \
		  Scanner.o

SOBJS		= $(SLICE_OBJS) \
		  PersistentFilesystemI.o \
		  Server.o

OBJS		= $(COBJS) \
		  $(SOBJS) \
		  $(COLOBJS)


include $(top_srcdir)/make/Make.rules

CPPFLAGS	:= -I. $(CPPFLAGS) $(READLINE_FLAGS)
SLICE2CPPFLAGS	:= -I. $(SLICE2CPPFLAGS)

$(CLIENT): $(COBJS)
	rm -f $@
	$(CXX) $(LDFLAGS) $(LDEXEFLAGS) -o $@ $(COBJS) $(LIBS) $(READLINE_LIBS)

$(SERVER): $(SOBJS)
	rm -f $@
	$(CXX) $(LDFLAGS) $(LDEXEFLAGS) -o $@ $(SOBJS) $(DB_RPATH_LINK) -lFreeze $(LIBS)

clean::
	-rm -rf db/*
