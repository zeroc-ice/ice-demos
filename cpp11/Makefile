# **********************************************************************
#
# Copyright (c) 2003-present ZeroC, Inc. All rights reserved.
#
# **********************************************************************

top_srcdir      := ..
lang_srcdir     := $(top_srcdir)/cpp11

include $(top_srcdir)/make/Make.rules
include $(lang_srcdir)/make/Make.rules

exclude := Ice/qt
demos := $(filter-out $(exclude),$(call demos-without-project-makefile,Client.cpp))
include $(shell find $(lang_srcdir) -name Makefile.mk)
$(foreach t,$(demos),$(eval $(call create-cpp-demo-project,$(t))))
$(call make-projects,$(projects))
