# **********************************************************************
#
# Copyright (c) 2003-present ZeroC, Inc. All rights reserved.
#
# **********************************************************************

$(demo)_libraries = HelloService

$(demo)_HelloService_sources            = HelloServiceI.cpp HelloI.cpp Hello.ice
$(demo)_HelloService_soversion          =
$(demo)_HelloService_version            =
$(demo)_HelloService_dependencies       = IceBox Ice

demos += $(demo)
