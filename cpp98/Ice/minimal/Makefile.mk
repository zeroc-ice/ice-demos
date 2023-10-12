#
# Copyright (c) ZeroC, Inc. All rights reserved.
#

$(demo)_programs = server client

$(demo)_server_sources     = Hello.ice Printer.cpp Server.cpp

demos += $(demo)
