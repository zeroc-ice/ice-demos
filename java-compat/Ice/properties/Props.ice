// **********************************************************************
//
// Copyright (c) 2003-2016 ZeroC, Inc. All rights reserved.
//
// **********************************************************************

#ifndef DEMO_ICE
#define DEMO_ICE

#include <Ice/Properties.ice>

module Demo
{

interface Props
{
    idempotent Ice::PropertyDict getChanges();
    void shutdown();
};

};

#endif
