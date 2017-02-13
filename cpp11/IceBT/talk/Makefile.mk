# **********************************************************************
#
# Copyright (c) 2003-2017 ZeroC, Inc. All rights reserved.
#
# This copy of Ice is licensed to you under the terms described in the
# ICE_LICENSE file included in this distribution.
#
# **********************************************************************

$(demo)_programs	= talk
$(demo)_dependencies	= IceBT++11 Ice++11

$(demo)_talk_sources	= App.cpp Talk.ice

ifneq ($(filter debian ubuntu,$(linux_id)),)
demos += $(demo)
endif
