//
// Copyright (c) ZeroC, Inc. All rights reserved.
//

#pragma once

#include <Ice/PropertiesAdmin.ice>

module Demo
{
    interface Properties
    {
        idempotent Ice::PropertyDict getChanges();
        void shutdown();
    }
}
