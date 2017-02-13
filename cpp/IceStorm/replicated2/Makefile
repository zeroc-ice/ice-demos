# **********************************************************************
#
# Copyright (c) 2003-2017 ZeroC, Inc. All rights reserved.
#
# **********************************************************************

top_srcdir	= ../..

PUBLISHER	= publisher
SUBSCRIBER	= subscriber

TARGETS		= $(PUBLISHER) $(SUBSCRIBER)

SLICE_OBJS	= Clock.o

POBJS		= $(SLICE_OBJS) \
		  Publisher.o

SOBJS		= $(SLICE_OBJS) \
		  Subscriber.o

OBJS		= $(POBJS) \
		  $(SOBJS)

include $(top_srcdir)/make/Make.rules

CPPFLAGS	:= -I. $(CPPFLAGS)
LIBS		:= -lIceStorm $(LIBS)

$(PUBLISHER): $(POBJS)
	rm -f $@
	$(CXX) $(LDFLAGS) $(LDEXEFLAGS) -o $@ $(POBJS) $(LIBS)

$(SUBSCRIBER): $(SOBJS)
	rm -f $@
	$(CXX) $(LDFLAGS) $(LDEXEFLAGS) -o $@ $(SOBJS) $(LIBS)

clean::
	-rm -rf db1/*
	-rm -rf db2/*
	-rm -rf db3/*
