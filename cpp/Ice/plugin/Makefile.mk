# **********************************************************************
#
# Copyright (c) 2003-2015 ZeroC, Inc. All rights reserved.
#
# This copy of Ice is licensed to you under the terms described in the
# ICE_LICENSE file included in this distribution.
#
# **********************************************************************

$(demo)_libraries 		= HelloPlugin LoggerPlugin

$(demo)_server_sources		= Server.cpp Hello.ice
$(demo)_HelloPlugin_sources	= HelloPluginI.cpp Hello.ice
$(demo)_LoggerPlugin_sources	= LoggerPluginI.cpp

demos += $(demo)
