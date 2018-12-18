// **********************************************************************
//
// Copyright (c) 2003-present ZeroC, Inc. All rights reserved.
//
// **********************************************************************

#pragma once

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
