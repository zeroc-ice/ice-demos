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
		  plugin \
		  interleaved \
		  properties \
		  optional \
		  context

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
