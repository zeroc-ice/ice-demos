# **********************************************************************
#
# Copyright (c) 2003-2015 ZeroC, Inc. All rights reserved.
#
# This copy of Ice is licensed to you under the terms described in the
# ICE_LICENSE file included in this distribution.
#
# **********************************************************************

$(demo)_dependencies	= IceObjC Glacier2ObjC

$(demo)_client_sources 	= Library.ice \
		  	  Session.ice \
		  	  Glacier2Session.ice \
			  Client.m \
		  	  Grammar.m \
		  	  Scanner.m \
		  	  Parser.m \
	  	  	  RunParser.m

demos += $(demo)
