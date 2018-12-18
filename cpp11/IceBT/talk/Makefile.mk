# **********************************************************************
#
# Copyright (c) 2003-present ZeroC, Inc. All rights reserved.
#
# **********************************************************************

ifneq ($(IceBT_system_libs),)
$(demo)_programs        = talk
$(demo)_dependencies    = IceBT Ice

$(demo)_talk_sources    = App.cpp Talk.ice

demos += $(demo)
endif
