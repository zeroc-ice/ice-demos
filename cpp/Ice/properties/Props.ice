//
// Copyright (c) ZeroC, Inc. All rights reserved.
//

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
