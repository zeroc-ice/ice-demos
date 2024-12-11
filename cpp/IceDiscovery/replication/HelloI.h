//
// Copyright (c) ZeroC, Inc. All rights reserved.
//

#ifndef HELLO_I_H
#define HELLO_I_H

#include <Hello.h>

class HelloI : public Demo::Hello
{
public:
    HelloI(std::string);

    std::string getGreeting(const Ice::Current&) override;
    void shutdown(const Ice::Current&) override;

private:
    const std::string _name;
};

#endif
