# **********************************************************************
#
# Copyright (c) 2003-2016 ZeroC, Inc. All rights reserved.
#
# **********************************************************************

#
# Determines whether the extension uses PHP namespaces (requires
# PHP 5.3 or later).
#
!if "$(USE_NAMESPACES)" == ""
USE_NAMESPACES		= no
!endif

# ----------------------------------------------------------------------
# Don't change anything below this line!
# ----------------------------------------------------------------------

VERSION		= 3.7a2

#
# Common definitions
#
ice_language     = php
slice_translator = slice2php.exe

!if "$(PROCESSOR_ARCHITECTURE)" == "AMD64"
ice_bin_dist_dir = $(PROGRAMFILES) (x86)\ZeroC\Ice-$(VERSION)
!else
ice_bin_dist_dir = $(PROGRAMFILES)\ZeroC\Ice-$(VERSION)
!endif

!if "$(ICE_HOME)" == ""

!if !exist ("$(ice_bin_dist_dir)\bin\$(slice_translator)")
!error Unable to find a valid Ice distribution, please verify ICE_HOME is properly configured and Ice is correctly installed.
!endif

ice_dir = $(ice_bin_dist_dir)
ice_cpp_dir = $(ice_bin_dist_dir)

!else

ice_dir = $(ICE_HOME)

!if exist ("$(ice_dir)\bin\$(slice_translator)")

ice_cpp_dir = $(ice_dir)

!else

!if exist ("$(ice_dir)\cpp\bin\$(slice_translator)")
ice_cpp_dir = $(ice_dir)\cpp
!else
!error Unable to find $(slice_translator) in $(ICE_HOME)\bin, please verify ICE_HOME is properly configured and Ice is correctly installed.
!endif

!endif

!endif

slicedir		= $(ice_dir)\slice
SLICE2PHP 		= $(ice_cpp_dir)\bin\$(slice_translator)

ICECPPFLAGS		= -I"$(slicedir)"
SLICE2PHPFLAGS		= $(ICECPPFLAGS)

!if "$(USE_NAMESPACES)" == "yes"
SLICE2PHPFLAGS		= $(SLICE2PHPFLAGS) -n
!endif

EVERYTHING		= all clean install depend

.SUFFIXES:
.SUFFIXES:		.cpp .obj .php .res .rc .d .ice

all::

!if exist(.depend.mak)
depend::
	@del /q .depend.mak

!include .depend.mak
!endif

!if "$(SLICE_SRCS)" != ""

depend:: $(SLICE_SRCS:.ice=.d)

$(SLICE_SRCS:.ice=.php): "$(SLICE2PHP)"

all:: $(SLICE_SRCS:.ice=.php)

clean::
	del /q $(SLICE_SRCS:.ice=.php)

!endif

.ice.php:
	"$(SLICE2PHP)" $(SLICE2PHPFLAGS) $<

.ice.d:
	@echo Generating dependencies for $<
	@"$(SLICE2PHP)" $(SLICE2PHPFLAGS) --depend $< | \
	cscript /NoLogo $(top_srcdir)\make\makedepend-slice.vbs $(*F).ice

install::
