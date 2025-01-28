// Copyright (c) ZeroC, Inc.

#pragma once

["java:package:com.zeroc.demos.Ice.context"]
module Demo
{
    interface Context
    {
        idempotent void call();
        void shutdown();
    }
}
