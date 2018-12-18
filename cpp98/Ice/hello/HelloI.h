// **********************************************************************
//
// Copyright (c) 2003-present ZeroC, Inc. All rights reserved.
//
// **********************************************************************

#ifndef HELLO_I_H
#define HELLO_I_H

#include <Hello.h>

class HelloI : public Demo::Hello
{
public:

    virtual void sayHello(int delay, const Ice::Current&);
    virtual void shutdown(const Ice::Current&);
};

#endif
