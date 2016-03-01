// **********************************************************************
//
// Copyright (c) 2003-2016 ZeroC, Inc. All rights reserved.
//
// **********************************************************************

#pragma once

module Demo
{

interface CallbackReceiver
{
    void callback();
};

interface CallbackSender
{
    void initiateCallback(CallbackReceiver* proxy);
    void shutdown();
};

};

