// **********************************************************************
//
// Copyright (c) 2003-2016 ZeroC, Inc. All rights reserved.
//
// **********************************************************************

#ifndef CONTEXT_I_H
#define CONTEXT_I_H

#include <Context.h>

class ContextI : public Demo::Context
{
public:

    virtual void call(const Ice::Current&);
    virtual void shutdown(const Ice::Current&);
};

#endif
