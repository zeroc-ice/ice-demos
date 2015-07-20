# **********************************************************************
#
# Copyright (c) 2003-2015 ZeroC, Inc. All rights reserved.
#
# **********************************************************************

top_srcdir	= .

include $(top_srcdir)/make/Make.rules


ifeq ($(BUILD_WITH_ICEE),yes)

SUBDIRS		:= Ice IceDiscovery Glacier2 Chat

endif

ifneq ($(findstring MINGW,$(UNAME)),)
SUBDIRS		= Ice \
		  IceDiscovery \
		  Manual
endif

ifeq ($(SUBDIRS),)
SUBDIRS		= IceUtil \
		  Chat \
		  Freeze \
		  Glacier2 \
		  Ice \
		  IceBox \
		  IceDiscovery \
		  IceGrid \
		  IceStorm \
		  Manual
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
