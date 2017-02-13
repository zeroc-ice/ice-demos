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
ifeq ($(BUILD_WITH_ICEE),yes)
    LIBS		:= -Wl,-Bdynamic -lIceStorm -lGlacier2 $(LIBS)
else
    LIBS		:= -lIceStorm -lGlacier2 $(LIBS)
endif

$(PUBLISHER): $(POBJS)
	rm -f $@
	$(CXX) $(LDFLAGS) $(LDEXEFLAGS) -o $@ $(POBJS) $(LIBS)

$(SUBSCRIBER): $(SOBJS)
	rm -f $@
	$(CXX) $(LDFLAGS) $(LDEXEFLAGS) -o $@ $(SOBJS) $(LIBS) -lIceGrid

clean::
	-rm -rf db/node/* db/registry/* db/*.db
