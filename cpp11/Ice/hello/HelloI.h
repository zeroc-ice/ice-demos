//
// Copyright (c) ZeroC, Inc. All rights reserved.
//

#ifndef HELLO_I_H
#define HELLO_I_H

#include <functional>
#include <Hello.h>

class HelloI : public Demo::Hello
{
public:
    HelloI(const std::function<void()>& requestShutdown);

    virtual void sayHello(int delay, const Ice::Current&) override;
    virtual void shutdown(const Ice::Current&) override;

private:
    std::function<void()> _requestShutdown;
};

#endif
