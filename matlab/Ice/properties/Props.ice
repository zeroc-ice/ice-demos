// Copyright (c) ZeroC, Inc.

#pragma once

#include <Ice/Properties.ice>

module Demo
{
    interface Props
    {
        idempotent Ice::PropertyDict getChanges();
        void shutdown();
    }
}
