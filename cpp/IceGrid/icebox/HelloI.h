// **********************************************************************
//
// Copyright (c) 2003-2016 ZeroC, Inc. All rights reserved.
//
// **********************************************************************

#ifndef HELLO_I_H
#define HELLO_I_H

#include <Hello.h>

class HelloI : public Demo::Hello
{
public:

    HelloI(const std::string&);

    virtual void sayHello(const Ice::Current&);

private:

	// Required to prevent compiler warnings with MSVC++
	HelloI& operator=(const HelloI&);

    const std::string _serviceName;
};

#endif
