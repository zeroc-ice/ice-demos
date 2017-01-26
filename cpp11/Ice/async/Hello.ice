// **********************************************************************
//
// Copyright (c) 2003-2016 ZeroC, Inc. All rights reserved.
//
// **********************************************************************

#pragma once

module Demo
{

exception RequestCanceledException
{
};

interface Hello
{
    ["amd"] idempotent void sayHello(int delay)
        throws RequestCanceledException;

    void shutdown();
};

};
