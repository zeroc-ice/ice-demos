# **********************************************************************
#
# Copyright (c) 2003-2016 ZeroC, Inc. All rights reserved.
#
# **********************************************************************

top_srcdir = ..

SLICE_SRCS 	= Chat.ice \
		  PollingChat.ice

include $(top_srcdir)/make/Make.rules

SLICE2PHPFLAGS  := -I. $(SLICE2PHPFLAGS)

SRCS 		= Chat.php \
		  PollingChat.php

all:: $(SRCS)

%.php: %.ice
	$(SLICE2PHP) $(SLICE2PHPFLAGS) $<

clean::
	rm -f $(SRCS)
