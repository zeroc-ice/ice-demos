# **********************************************************************
#
# Copyright (c) 2003-2015 ZeroC, Inc. All rights reserved.
#
# This copy of Ice is licensed to you under the terms described in the
# ICE_LICENSE file included in this distribution.
#
# **********************************************************************

$(demo)_programs	= server client1 client2
$(demo)_server_sources	= Server.cpp Greet.ice GreetI.cpp
$(demo)_client1_sources	= ClientWithConverter.cpp Greet.ice Client.cpp StringConverterI.cpp
$(demo)_client2_sources	= ClientWithoutConverter.cpp Greet.ice Client.cpp

demos += $(demo)
