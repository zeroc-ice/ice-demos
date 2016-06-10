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

    HelloI(const std::shared_ptr<WorkQueue>&);
    virtual void sayHelloAsync(int, std::function<void()>, std::function<void(std::exception_ptr)>,
                               const Ice::Current&);
    virtual void shutdown(const Ice::Current&);

private:

    std::shared_ptr<WorkQueue> _workQueue;
};


#endif
