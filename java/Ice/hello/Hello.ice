// **********************************************************************
//
// Copyright (c) 2003-present ZeroC, Inc. All rights reserved.
//
// **********************************************************************

#pragma once

["java:package:com.zeroc.demos.Ice.hello"]
module Demo
{
    interface Hello
    {
        idempotent void sayHello(int delay);
        void shutdown();
    }
}
