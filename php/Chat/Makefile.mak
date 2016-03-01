# **********************************************************************
#
# Copyright (c) 2003-2016 ZeroC, Inc. All rights reserved.
#
# This copy of Ice is licensed to you under the terms described in the
# ICE_LICENSE file included in this distribution.
#
# **********************************************************************

top_srcdir	= ..

SLICE_SRCS		= Chat.ice \
			  PollingChat.ice

!include $(top_srcdir)\make\Make.rules.mak

SLICE2PHPFLAGS		= $(SLICE2PHPFLAGS) -I.
