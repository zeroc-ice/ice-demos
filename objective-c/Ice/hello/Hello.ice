// **********************************************************************
//
// Copyright (c) 2003-2017 ZeroC, Inc. All rights reserved.
//
// **********************************************************************

#pragma once

module Demo
{
    interface Hello
    {
        idempotent void sayHello(int delay);
        void shutdown();
    }
}
