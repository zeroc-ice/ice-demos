//
// Copyright (c) ZeroC, Inc. All rights reserved.
//

#pragma once

["java:package:com.zeroc.demos.Ice.interrupt"]
module Demo
{
    interface TaskManager
    {
        void run(int id);
        void shutdown();
    }
}
