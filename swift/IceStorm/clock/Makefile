#
# Copyright (c) ZeroC, Inc. All rights reserved.
#

# Use the slice2swift compiler to generate Swift code from the Slice files of this demo.

SLICE2SWIFT ?= $(or $(shell command -v slice2swift 2>/dev/null),$(ICE_HOME)/bin/slice2swift)

ICE_FILES := $(wildcard Sources/*.ice)
SLICE_NAME := $(basename $(notdir $(ICE_FILES)))

GENERATED_SWIFT := Sources/Publisher/$(SLICE_NAME).swift Sources/Subscriber/$(SLICE_NAME).swift

all: slice

slice: $(GENERATED_SWIFT)

Sources/Publisher/%.swift: Sources/%.ice
	$(SLICE2SWIFT) --output-dir $(dir $@) $<

Sources/Subscriber/%.swift: Sources/%.ice
	$(SLICE2SWIFT) --output-dir $(dir $@) $<

clean:
	rm -f $(GENERATED_SWIFT)
	-rm -rf .build
