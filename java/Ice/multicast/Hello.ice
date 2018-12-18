// **********************************************************************
//
// Copyright (c) 2003-present ZeroC, Inc. All rights reserved.
//
// **********************************************************************

#pragma once

["java:package:com.zeroc.demos.Ice.multicast"]
module Demo
{
    interface Hello
    {
        idempotent void sayHello();
    }
}
