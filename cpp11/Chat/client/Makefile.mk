# **********************************************************************
#
# Copyright (c) 2003-present ZeroC, Inc. All rights reserved.
#
# **********************************************************************

$(demo)_programs        = chatgl2client chatpollclient
$(demo)_dependencies    = Ice Glacier2

$(demo)_chatpollclient_sources  = Chat.ice PollingChat.ice ChatUtils.cpp PollingClient.cpp
$(demo)_chatgl2client_sources   = Chat.ice ChatSession.ice ChatUtils.cpp Client.cpp

demos += $(demo)
