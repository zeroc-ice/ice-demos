//
// Copyright (c) ZeroC, Inc. All rights reserved.
//

#pragma once

module Demo
{
    ["clr:serializable:Demo.MyGreeting"] sequence<byte> Greeting;

    interface Greet
    {
        idempotent void sendGreeting(Greeting g);
        void shutdown();
    }
}
