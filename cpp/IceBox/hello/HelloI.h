// Copyright (c) ZeroC, Inc.

#ifndef HELLO_I_H
#define HELLO_I_H

#include <Hello.h>

class HelloI : public Demo::Hello
{
public:
    void sayHello(const Ice::Current&) override;
};

#endif
