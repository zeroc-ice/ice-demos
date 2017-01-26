# **********************************************************************
#
# Copyright (c) 2003-2015 ZeroC, Inc. All rights reserved.
#
# This copy of Ice is licensed to you under the terms described in the
# ICE_LICENSE file included in this distribution.
#
# **********************************************************************

$(demo)_libraries = RegistryPlugin

$(demo)_RegistryPlugin_sources		= RegistryPlugin.cpp
$(demo)_RegistryPlugin_dependencies 	= IceGrid Glacier2 Ice

$(demo)_client_dependencies		= IceGrid Glacier2 Ice



demos += $(demo)
