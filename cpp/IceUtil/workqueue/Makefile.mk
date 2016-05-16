# **********************************************************************
#
# Copyright (c) 2003-2015 ZeroC, Inc. All rights reserved.
#
# This copy of Ice is licensed to you under the terms described in the
# ICE_LICENSE file included in this distribution.
#
# **********************************************************************

$(demo)_programs	= workqueue
$(demo)_dependencies	= Ice

$(demo)_workqueue_sources = WorkQueue.cpp

demos += $(demo)
