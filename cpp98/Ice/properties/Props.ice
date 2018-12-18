// **********************************************************************
//
// Copyright (c) 2003-present ZeroC, Inc. All rights reserved.
//
// **********************************************************************

#pragma once
#include <Ice/Properties.ice>

module Demo
{
    interface Props
    {
        idempotent Ice::PropertyDict getChanges();
        void shutdown();
    }
}
