// Copyright (c) ZeroC, Inc.

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
