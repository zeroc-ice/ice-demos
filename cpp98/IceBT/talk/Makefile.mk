# **********************************************************************
#
# Copyright (c) 2003-2017 ZeroC, Inc. All rights reserved.
#
# **********************************************************************

$(demo)_programs        = talk
$(demo)_dependencies    = IceBT Ice

$(demo)_talk_sources    = App.cpp Talk.ice

ifneq ($(filter debian ubuntu,$(linux_id)),)
demos += $(demo)
endif
