# **********************************************************************
#
# Copyright (c) 2003-present ZeroC, Inc. All rights reserved.
#
# **********************************************************************

$(demo)_libraries               = HelloPlugin LoggerPlugin

$(demo)_server_sources          = Server.cpp Hello.ice
$(demo)_HelloPlugin_sources     = HelloPluginI.cpp Hello.ice
$(demo)_LoggerPlugin_sources    = LoggerPluginI.cpp

demos += $(demo)
