// Copyright (c) ZeroC, Inc.

#pragma once

["java:package:com.zeroc.demos.Ice.serialize"]
module Demo
{
    ["java:serializable:MyGreeting"] sequence<byte> Greeting;

    interface Greet
    {
        idempotent void sendGreeting(Greeting g);
        void shutdown();
    }
}
