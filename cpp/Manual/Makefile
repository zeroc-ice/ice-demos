# **********************************************************************
#
# Copyright (c) 2003-2017 ZeroC, Inc. All rights reserved.
#
# **********************************************************************

top_srcdir	= ..

include $(top_srcdir)/make/Make.rules

ifeq ($(BUILD_WITH_ICEE),yes)
SUBDIRS		= printer simpleFilesystem lifecycle
    ifeq ($(BUILD_DEMOS),dynamic)
    	SUBDIRS		:= $(SUBDIRS) evictorFilesystem mapFilesystem
    endif
else

ifneq ($(findstring MINGW,$(UNAME)),)
SUBDIRS		= printer \
		  simpleFilesystem
else
SUBDIRS		= printer \
		  simpleFilesystem \
		  lifecycle \
		  evictorFilesystem \
                  mapFilesystem
endif
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
