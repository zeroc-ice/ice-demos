// **********************************************************************
//
// Copyright (c) 2003-2015 ZeroC, Inc. All rights reserved.
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
};

};
