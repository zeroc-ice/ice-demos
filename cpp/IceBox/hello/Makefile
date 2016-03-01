# **********************************************************************
#
# Copyright (c) 2003-2016 ZeroC, Inc. All rights reserved.
#
# **********************************************************************

top_srcdir	= ../..

CLIENT		= client

LIBFILENAME     = $(call mklibfilename,HelloService)
SONAME          = $(call mksoname,HelloService)  

TARGETS		= $(CLIENT) $(LIBFILENAME)

SLICE_OBJS	= Hello.o

COBJS		= $(SLICE_OBJS) \
		  Client.o

SOBJS		= $(SLICE_OBJS) \
		  HelloI.o \
		  HelloServiceI.o

OBJS		= $(COBJS) \
		  $(SOBJS)

include $(top_srcdir)/make/Make.rules

CPPFLAGS	:= -I. $(CPPFLAGS)

ifeq ($(BUILD_WITH_ICEE),yes)
LINKWITH	:= -Wl,-Bdynamic
endif
LINKWITH	:= $(LINKWITH) -lIceBox $(BZIP2_RPATH_LINK) -lIce -lIceUtil

$(LIBFILENAME): $(SOBJS)
	rm -f $@
	$(call mkshlib,$@,$(SONAME),$(SOBJS),$(LINKWITH))

$(CLIENT): $(COBJS)
	rm -f $@
	$(CXX) $(LDFLAGS) $(LDEXEFLAGS) -o $@ $(COBJS) $(LIBS)
