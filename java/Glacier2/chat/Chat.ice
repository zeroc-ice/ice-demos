// **********************************************************************
//
// Copyright (c) 2003-2015 ZeroC, Inc. All rights reserved.
//
// ICE_LICENSE file included in this distribution.
//
// **********************************************************************

#pragma once

#include <Glacier2/Session.ice>

module Demo
{

interface ChatCallback
{
    void message(string data);
};

interface ChatSession extends Glacier2::Session
{
    void setCallback(ChatCallback* callback);
    void say(string data);
};

};
