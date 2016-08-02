// **********************************************************************
//
// Copyright (c) 2003-2016 ZeroC, Inc. All rights reserved.
//
// **********************************************************************

#ifndef WORK_QUEUE_H
#define WORK_QUEUE_H

#include <Hello.h>

#include <list>
#include <condition_variable>
#include <mutex>
#include <thread>

class WorkQueue
{
public:

    WorkQueue();

    virtual void run();

    void add(int, std::function<void ()>, std::function<void (std::exception_ptr)>);
    void destroy();
    void start();
    void join();

private:

    using CallbackEntry = std::tuple<
        int,
        std::function<void ()>,
        std::function<void (std::exception_ptr)>>;

    std::mutex _mutex;
    std::condition_variable _condition;

    std::list<CallbackEntry> _callbacks;
    bool _done;
    std::thread _thread;
};

#endif
