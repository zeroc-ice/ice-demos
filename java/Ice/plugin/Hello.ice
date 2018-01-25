// **********************************************************************
//
// Copyright (c) 2003-2018 ZeroC, Inc. All rights reserved.
//
// **********************************************************************

#pragma once

module Demo
{
    interface Hello
    {
        idempotent void sayHello();
        void shutdown();
    }
}
