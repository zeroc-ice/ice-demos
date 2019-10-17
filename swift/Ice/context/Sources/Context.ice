//
// Copyright (c) ZeroC, Inc. All rights reserved.
//

#pragma once

module Demo
{
    interface Context
    {
        idempotent void call();
        void shutdown();
    }
}
