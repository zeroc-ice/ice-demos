# **********************************************************************
#
# Copyright (c) 2003-2015 ZeroC, Inc. All rights reserved.
#
# This copy of Ice is licensed to you under the terms described in the
# ICE_LICENSE file included in this distribution.
#
# **********************************************************************

$(demo)_libraries = HelloService

$(demo)_HelloService_sources		= HelloServiceI.cpp HelloI.cpp Hello.ice
$(demo)_HelloService_soversion		=
$(demo)_HelloService_version		=
$(demo)_HelloService_dependencies	= IceBox Ice

demos += $(demo)
