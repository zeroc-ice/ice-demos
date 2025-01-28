// Copyright (c) ZeroC, Inc.

#pragma once

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
