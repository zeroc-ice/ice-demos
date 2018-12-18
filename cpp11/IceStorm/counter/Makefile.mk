# **********************************************************************
#
# Copyright (c) 2003-present ZeroC, Inc. All rights reserved.
#
# **********************************************************************

$(demo)_dependencies    = IceStorm Ice

$(demo)_client_sources  = Client.cpp Counter.ice CounterObserverI.cpp

demos += $(demo)
