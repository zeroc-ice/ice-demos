// **********************************************************************
//
// Copyright (c) 2003-2016 ZeroC, Inc. All rights reserved.
//
// **********************************************************************

#pragma once

#include <Ice/Identity.ice>

module Demo
{

interface CallbackReceiver
{
    void callback(int num);
};

interface CallbackSender
{
    void addClient(Ice::Identity ident);
};

};

