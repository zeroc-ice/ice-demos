// Copyright (c) ZeroC, Inc.

#pragma once

["java:package:com.zeroc.demos.IceDiscovery.hello"]
module Demo
{
    interface Hello
    {
        idempotent void sayHello(int delay);
        void shutdown();
    }
}
