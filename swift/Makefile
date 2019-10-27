#
# Copyright (c) ZeroC, Inc. All rights reserved.
#

top_srcdir := ..

include $(top_srcdir)/make/Make.rules

CONFIG ?= $(if $(filter $(OPTIMIZE),no),Debug,Release)
PROJECT ?= demos.xcodeproj

# $(call make-xcodebuild-rule,$1=rule,$2=platform,$3=scheme,$4=target)
define make-xcodebuild-rule
$1:: $(PROJECT)
	xcodebuild -project $(PROJECT) \
                   -scheme "$3 $(if $(filter $2,macosx),macOS,iOS)" \
                   -configuration $(CONFIG) \
                   -sdk $2 $4
endef

demos.xcodeproj: Rakefile | carthage
	rake

all:: demos

carthage:
	carthage update --cache-builds

demos:: carthage

install::
	@echo nothing to install

$(foreach p,$(platforms),$(eval $(call make-xcodebuild-rule,demos,$p,allDemos,build)))

$(foreach p,$(platforms),$(eval $(call make-xcodebuild-rule,clean,$p,allDemos,clean)))

$(foreach p,$(supported-platforms),$(eval $(call make-xcodebuild-rule,distclean,$p,allDemos,clean)))
