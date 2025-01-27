// Copyright (c) ZeroC, Inc.

#pragma once

module Demo
{
    interface Hello
    {
        idempotent string getGreeting();
        void shutdown();
    }
}
