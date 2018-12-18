// **********************************************************************
//
// Copyright (c) 2003-present ZeroC, Inc. All rights reserved.
//
// **********************************************************************

#ifndef WORK_QUEUE_H
#define WORK_QUEUE_H

#include <Hello.h>
#include <Ice/Ice.h>

#include <list>

class WorkQueue : public IceUtil::Thread
{
public:

    WorkQueue();

    virtual void run();

    void add(const Demo::AMD_Hello_sayHelloPtr&, int);
    void destroy();

private:

    struct CallbackEntry
    {
        Demo::AMD_Hello_sayHelloPtr cb;
        int delay;
    };

    IceUtil::Monitor<IceUtil::Mutex> _monitor;
    std::list<CallbackEntry> _callbacks;
    bool _done;
};

typedef IceUtil::Handle<WorkQueue> WorkQueuePtr;

#endif
