// **********************************************************************
//
// Copyright (c) 2003-2018 ZeroC, Inc. All rights reserved.
//
// **********************************************************************

#pragma once

["js:module:bidir"]
module Demo
{
    interface CallbackReceiver
    {
        void callback(int num);
    }

    interface CallbackSender
    {
        void addClient(CallbackReceiver* ident);
    }
}
