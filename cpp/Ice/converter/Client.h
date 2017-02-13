// **********************************************************************
//
// Copyright (c) 2003-2017 ZeroC, Inc. All rights reserved.
//
// **********************************************************************

#ifndef DEMO_CLIENT_H
#define DEMO_CLIENT_H

#include <Ice/Ice.h>

namespace Demo
{

class Client : public Ice::Application
{
    virtual int run(int, char*[]);
};

}

#endif
