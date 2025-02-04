// Copyright (c) ZeroC, Inc.

#pragma once

module Demo
{
    dictionary<string, string> PropertyDict;

    interface Props
    {
        idempotent PropertyDict getChanges();
        void shutdown();
    }
}
