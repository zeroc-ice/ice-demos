// Copyright (c) ZeroC, Inc.

#ifndef CHATBOT_H
#define CHATBOT_H

#include "Greeter.h"

namespace GreeterServer
{
    /// A Chatbot is an Ice servant that implements Slice interface Greeter.
    class Chatbot : public VisitorCenter::Greeter
    {
    public:
        std::string greet(std::string name, const Ice::Current&) override;
    };
}

#endif
