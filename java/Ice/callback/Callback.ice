// Copyright (c) ZeroC, Inc.

#pragma once

["java:package:com.zeroc.demos.Ice.callback"]
module Demo
{
    interface CallbackReceiver
    {
        void callback();
    }
    interface CallbackSender
    {
        void initiateCallback(CallbackReceiver* proxy);
        void shutdown();
    }
}
