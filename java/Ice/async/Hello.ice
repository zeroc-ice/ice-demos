//
// Copyright (c) ZeroC, Inc. All rights reserved.
//

#pragma once

["java:package:com.zeroc.demos.Ice.async"]
module Demo
{
    exception RequestCanceledException
    {
    }

    interface Hello
    {
        ["amd"] idempotent void sayHello(int delay)
            throws RequestCanceledException;

        void shutdown();
    }
}
