// **********************************************************************
//
// Copyright (c) 2003-2016 ZeroC, Inc. All rights reserved.
//
// **********************************************************************

#ifndef ECHO_I_H
#define ECHO_I_H

#include <Greet.h>

class GreetI : public Demo::Greet
{
public:

    virtual std::string exchangeGreeting(const std::string&, const Ice::Current&);
    virtual void shutdown(const Ice::Current&);
};

#endif
