// **********************************************************************
//
// Copyright (c) 2003-present ZeroC, Inc. All rights reserved.
//
// **********************************************************************

#pragma once

#include <Ice/Identity.ice>

module Demo
{
    interface CallbackReceiver
    {
        void callback(int num);
    }

    interface CallbackSender
    {
        void addClient(CallbackReceiver* receiver);
    }
}
