# **********************************************************************
#
# Copyright (c) 2003-present ZeroC, Inc. All rights reserved.
#
# **********************************************************************

$(demo)_server_sources  = Server.cpp Nrvo.ice MyStringSeq.cpp NrvoI.cpp
$(demo)_client_sources  = Client.cpp Nrvo.ice MyStringSeq.cpp

demos += $(demo)
