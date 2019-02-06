//
// Copyright (c) ZeroC, Inc. All rights reserved.
//

#pragma once

["java:package:com.zeroc.demos.Glacier2.callback"]
module Demo
{
    interface CallbackReceiver
    {
        void callback();
    }

    interface Callback
    {
        void initiateCallback(CallbackReceiver* proxy);
        void shutdown();
    }
}
