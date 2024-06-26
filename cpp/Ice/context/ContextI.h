//
// Copyright (c) ZeroC, Inc. All rights reserved.
//

#ifndef CONTEXT_I_H
#define CONTEXT_I_H

#include <Context.h>

class ContextI : public Demo::Context
{
public:
    virtual void call(const Ice::Current&) override;
    virtual void shutdown(const Ice::Current&) override;
};

#endif
