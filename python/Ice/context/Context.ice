// Copyright (c) ZeroC, Inc.

#pragma once

module Demo
{
    interface Context
    {
        idempotent void call();
        void shutdown();
    }
}
