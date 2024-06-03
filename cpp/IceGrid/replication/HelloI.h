//
// Copyright (c) ZeroC, Inc. All rights reserved.
//

#ifndef HELLO_I_H
#define HELLO_I_H

#include <Hello.h>

class HelloI final : public Demo::Hello
{
public:
    HelloI(std::string_view);

    std::string getGreeting(const Ice::Current&) final;
    void shutdown(const Ice::Current&) final;

private:
    const std::string _name;
};

#endif
