// Copyright (c) ZeroC, Inc.

#pragma once

["java:package:com.zeroc.demos.Ice.plugin"]
module Demo
{
    interface Hello
    {
        idempotent void sayHello();
        void shutdown();
    }
}
