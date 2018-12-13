// **********************************************************************
//
// Copyright (c) 2003-2018 ZeroC, Inc. All rights reserved.
//
// **********************************************************************

#pragma once

#include <Ice/Properties.ice>

["java:package:com.zeroc.demos.Ice.properties"]
module Demo
{
    interface Props
    {
        idempotent Ice::PropertyDict getChanges();
        void shutdown();
    }
}
