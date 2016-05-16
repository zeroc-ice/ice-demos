# **********************************************************************
#
# Copyright (c) 2003-2016 ZeroC, Inc. All rights reserved.
#
# This copy of Ice is licensed to you under the terms described in the
# ICE_LICENSE file included in this distribution.
#
# **********************************************************************

top_srcdir 	:= ..
lang_srcdir	:= $(top_srcdir)/cpp11

include $(top_srcdir)/make/Make.rules
include $(lang_srcdir)/make/Make.rules

demos := $(call demos-without-project-makefile,Client.cpp)
include $(shell find $(lang_srcdir) -name Makefile.mk)
$(foreach t,$(demos),$(eval $(call create-cpp-demo-project,$(t))))
$(call make-projects,$(projects),make-cpp-demo-project,create-demo-project-targets)
