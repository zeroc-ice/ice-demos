# Copyright (c) ZeroC, Inc.

$(demo)_dependencies    = IceStorm Ice

$(demo)_client_sources  = Client.cpp Counter.ice CounterObserverI.cpp

demos += $(demo)
