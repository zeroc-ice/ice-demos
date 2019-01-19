//
// Copyright (c) ZeroC, Inc. All rights reserved.
//

#ifndef HELLO_I_H
#define HELLO_I_H

#include <Hello.h>

class HelloI : public Demo::Hello
{
public:

    HelloI(const std::string&);

    virtual std::string getGreeting(const Ice::Current&);
    virtual void shutdown(const Ice::Current&);

private:

    // Required to prevent compiler warnings with MSVC++
    HelloI& operator=(const HelloI&);

    const std::string _name;
};

#endif
