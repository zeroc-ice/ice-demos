// Copyright (c) ZeroC, Inc.

#ifndef HELLO_I_H
#define HELLO_I_H

#include "Hello.h"

class HelloI final : public Demo::Hello
{
public:
    HelloI(std::string_view);

    void sayHello(const Ice::Current&) final;

private:
    const std::string _name;
};

#endif
