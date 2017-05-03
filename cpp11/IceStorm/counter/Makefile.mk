# **********************************************************************
#
# Copyright (c) 2003-2017 ZeroC, Inc. All rights reserved.
#
# **********************************************************************

$(demo)_dependencies    = IceStorm Ice

$(demo)_client_sources  = Client.cpp Counter.ice CounterObserverI.cpp

demos += $(demo)
