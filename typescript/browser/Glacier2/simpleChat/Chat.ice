// **********************************************************************
//
// Copyright (c) 2003-present ZeroC, Inc. All rights reserved.
//
// **********************************************************************

#pragma once

#include <Glacier2/Session.ice>

//
// Map Slice modules to JavaScript ES6 modules
//
[["js:es6-module"]]

module Demo
{
    interface ChatCallback
    {
        void message(string data);
    }

    interface ChatSession extends Glacier2::Session
    {
        void setCallback(ChatCallback* callback);
        void say(string data);
    }
}
