# **********************************************************************
#
# Copyright (c) 2003-2016 ZeroC, Inc. All rights reserved.
#
# This copy of Ice is licensed to you under the terms described in the
# ICE_LICENSE file included in this distribution.
#
# **********************************************************************

top_srcdir	:= ..
lang_srcdir	:= $(top_srcdir)/php

include $(top_srcdir)/make/Make.rules
include $(lang_srcdir)/make/Make.rules

#
# Translate the Slice files from test directories
#
demos := $(call demos-without-project-makefile,*.ice)
$(foreach d,$(demos),$(eval $(call make-php-demo-project,$d)))
