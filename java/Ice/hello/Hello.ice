// Copyright (c) ZeroC, Inc.

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
