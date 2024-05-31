//
// Copyright (c) ZeroC, Inc. All rights reserved.
//

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
