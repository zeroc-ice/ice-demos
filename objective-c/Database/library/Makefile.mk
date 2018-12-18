# **********************************************************************
#
# Copyright (c) 2003-present ZeroC, Inc. All rights reserved.
#
# **********************************************************************

$(demo)_dependencies    = IceObjC Glacier2ObjC

$(demo)_client_sources  = Library.ice \
                          Session.ice \
                          Glacier2Session.ice \
                          Client.m \
                          Grammar.m \
                          Scanner.m \
                          Parser.m \
                          RunParser.m

demos += $(demo)
