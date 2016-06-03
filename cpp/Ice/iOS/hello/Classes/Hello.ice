// **********************************************************************
//
// Copyright (c) 2003-2016 ZeroC, Inc. All rights reserved.
//
// **********************************************************************

#ifndef HELLO_ICE
#define HELLO_ICE

module Demo
{

["ami"] interface Hello
{
    idempotent void sayHello(int delay);
    void shutdown();
};

};

#endif
