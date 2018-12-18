# **********************************************************************
#
# Copyright (c) 2003-present ZeroC, Inc. All rights reserved.
#
# **********************************************************************

top_srcdir      := ..
lang_srcdir     := $(top_srcdir)/objective-c

include $(top_srcdir)/make/Make.rules
include $(lang_srcdir)/make/Make.rules

demos := $(call demos-without-project-makefile,Client.m)
include $(shell find $(lang_srcdir) -name Makefile.mk)
$(foreach t,$(demos),$(eval $(call create-objc-demo-project,$(t))))
$(call make-projects,$(projects))
