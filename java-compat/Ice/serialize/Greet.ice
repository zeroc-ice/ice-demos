// **********************************************************************
//
// Copyright (c) 2003-present ZeroC, Inc. All rights reserved.
//
// **********************************************************************

#pragma once

module Demo
{
    ["java:serializable:Demo.MyGreeting"] sequence<byte> Greeting;

    interface Greet
    {
        idempotent void sendGreeting(Greeting g);
        void shutdown();
    }
}
