// Copyright (c) ZeroC, Inc.

#pragma once

["java:package:com.zeroc.demos.Ice.multicast"]
module Demo
{
    interface Hello
    {
        idempotent void sayHello();
    }
}
