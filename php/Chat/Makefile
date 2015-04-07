# **********************************************************************
#
# Copyright (c) 2003-2015 ZeroC, Inc. All rights reserved.
#
# This copy of Chat Demo is licensed to you under the terms described
# in the CHAT_DEMO_LICENSE file included in this distribution.
#
# **********************************************************************

top_srcdir = ..


SLICE_SRCS = Chat.ice \
	PollingChat.ice

include $(top_srcdir)/make/Make.rules

SLICE2PHPFLAGS  := -I. $(SLICE2PHPFLAGS)

SRCS = Chat.php \
	PollingChat.php

all:: $(SRCS)

%.php: %.ice
	$(SLICE2PHP) $(SLICE2PHPFLAGS) $<

clean::
	rm -f $(SRCS)
