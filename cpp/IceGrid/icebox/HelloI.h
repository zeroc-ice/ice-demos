// Copyright (c) ZeroC, Inc.

#ifndef HELLO_I_H
#define HELLO_I_H

#include <Hello.h>

class HelloI : public Demo::Hello
{
public:
    HelloI(std::string);

    void sayHello(const Ice::Current&) override;

private:
    const std::string _serviceName;
};

#endif
