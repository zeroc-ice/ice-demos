//
// Copyright (c) ZeroC, Inc. All rights reserved.
//

#pragma once

#include <Glacier2/Session.ice>

["java:package:com.zeroc.demos.Glacier2.simpleChat"]
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
