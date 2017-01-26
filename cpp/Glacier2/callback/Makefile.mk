# **********************************************************************
#
# Copyright (c) 2003-2015 ZeroC, Inc. All rights reserved.
#
# This copy of Ice is licensed to you under the terms described in the
# ICE_LICENSE file included in this distribution.
#
# **********************************************************************

$(demo)_client_dependencies	= Ice Glacier2
$(demo)_client_sources 		= Client.cpp Callback.ice CallbackI.cpp

demos += $(demo)
