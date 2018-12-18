// **********************************************************************
//
// Copyright (c) 2003-present ZeroC, Inc. All rights reserved.
//
// **********************************************************************

#pragma once

//
// Map Slice modules to JavaScript ES6 modules
//
[["js:es6-module"]]

module Demo
{
    interface Hello
    {
        idempotent void sayHello(int delay);
        void shutdown();
    }
}
