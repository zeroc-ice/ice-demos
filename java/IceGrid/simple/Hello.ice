// Copyright (c) ZeroC, Inc.

#pragma once

["java:package:com.zeroc.demos.IceGrid.simple"]
module Demo
{
    interface Hello
    {
        idempotent void sayHello();
        void shutdown();
    }
}
