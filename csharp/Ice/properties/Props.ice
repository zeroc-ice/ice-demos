//
// Copyright (c) ZeroC, Inc. All rights reserved.
//

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
