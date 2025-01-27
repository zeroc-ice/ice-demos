// Copyright (c) ZeroC, Inc.

#pragma once

#include <Ice/PropertyDict.ice>

["java:package:com.zeroc.demos.Ice.properties"]
module Demo
{
    interface Props
    {
        idempotent Ice::PropertyDict getChanges();
        void shutdown();
    }
}
