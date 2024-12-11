//
// Copyright (c) ZeroC, Inc. All rights reserved.
//

#ifndef HELLO_I_H
#define HELLO_I_H

#include <Hello.h>

class HelloI : public Demo::Hello
{
public:
    void sayHello(int delay, const Ice::Current&) override;
    void shutdown(const Ice::Current&) override;
};

#endif
