# **********************************************************************
#
# Copyright (c) 2003-2015 ZeroC, Inc. All rights reserved.
#
# **********************************************************************

top_srcdir	= ../..

APP		= talk

TARGETS		= $(APP)

SLICE_OBJS	= Talk.o

OBJS		= $(SLICE_OBJS) \
		  App.o

include $(top_srcdir)/make/Make.rules

CPPFLAGS	:= -I. $(CPPFLAGS)

ifeq ($(BUILD_WITH_ICEE),yes)
    ICEBT_LIBS = -Wl,-B$(BUILD_DEMOS)
endif
ICEBT_LIBS	+= -lIceBT

$(APP): $(OBJS)
	rm -f $@
	$(CXX) $(LDFLAGS) $(LDEXEFLAGS) -o $@ $(OBJS) $(ICEBT_LIBS) $(LIBS)
ifeq ($(BUILD_WITH_ICEE),yes)
	$(Q)$(STRIP) --strip-all --remove-section=.comment --remove-section=.note $(@)
endif
