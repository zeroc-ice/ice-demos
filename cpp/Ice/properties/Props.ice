// Copyright (c) ZeroC, Inc.

#pragma once

#include <Ice/PropertyDict.ice>

module Demo
{
    interface Props
    {
        idempotent Ice::PropertyDict getChanges();
        void shutdown();
    }
}
