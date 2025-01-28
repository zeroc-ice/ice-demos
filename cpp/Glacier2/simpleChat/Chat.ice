// Copyright (c) ZeroC, Inc.

#pragma once

#include <Glacier2/Session.ice>

module Demo
{
    interface ChatCallback
    {
        void message(string data);
    }

    interface ChatSession extends Glacier2::Session
    {
        void setCallback(ChatCallback* callback);
        void say(string data);
    }
}
