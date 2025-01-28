// Copyright (c) ZeroC, Inc.

#pragma once

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
