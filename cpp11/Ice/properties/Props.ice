// **********************************************************************
//
// Copyright (c) 2003-2015 ZeroC, Inc. All rights reserved.
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
