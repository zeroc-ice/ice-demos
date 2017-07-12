# **********************************************************************
#
# Copyright (c) 2003-2017 ZeroC, Inc. All rights reserved.
#
# **********************************************************************

$(demo)_programs        = talk
$(demo)_dependencies    = IceBT Ice

$(demo)_talk_sources    = App.cpp Talk.ice

ifeq ($(shell pkg-config --exists bluez dbus-1 2> /dev/null && echo yes),yes)
demos += $(demo)
endif
