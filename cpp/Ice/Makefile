# **********************************************************************
#
# Copyright (c) 2003-2015 ZeroC, Inc. All rights reserved.
#
# **********************************************************************

top_srcdir	= ..

include $(top_srcdir)/make/Make.rules

SUBDIRS		= minimal \
		  hello \
		  latency \
		  throughput \
		  value \
		  callback \
		  nested \
		  invoke \
		  bidir \
		  session \
		  converter \
		  async \
		  multicast \
		  nrvo \
		  properties \
		  optional \
		  context \
		  locator

ifneq ($(BUILD_WITH_ICEE),yes)
SUBDIRS		:= $(SUBDIRS) \
		   interleaved \
		   plugin
endif

ifeq ($(BUILD_DEMOS),dynamic)
SUBDIRS		:= $(SUBDIRS) \
		   plugin
endif

.PHONY: $(EVERYTHING) $(SUBDIRS)

all:: $(SUBDIRS)

$(SUBDIRS):
	@echo "making all in $@"
	@$(MAKE) all --directory=$@

$(EVERYTHING_EXCEPT_ALL)::
	@for subdir in $(SUBDIRS); \
	do \
	    echo "making $@ in $$subdir"; \
	    ( cd $$subdir && $(MAKE) $@ ) || exit 1; \
	done
