//
// Copyright (c) ZeroC, Inc. All rights reserved.
//

#ifndef CONTEXT_I_H
#define CONTEXT_I_H

#include <Context.h>

class ContextI : public Demo::Context
{
public:
    void call(const Ice::Current&) override;
    void shutdown(const Ice::Current&) override;
};

#endif
