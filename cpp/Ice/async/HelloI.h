// **********************************************************************
//
// Copyright (c) 2003-2016 ZeroC, Inc. All rights reserved.
//
// **********************************************************************

#ifndef HELLO_I_H
#define HELLO_I_H

#include <Hello.h>
#include <WorkQueue.h>

class HelloI : public Demo::Hello
{
public:

    HelloI(const WorkQueuePtr&);

    virtual void sayHello_async(const Demo::AMD_Hello_sayHelloPtr&, int, const Ice::Current&);
    virtual void shutdown(const Ice::Current&);

private:

    WorkQueuePtr _workQueue;
};


#endif
