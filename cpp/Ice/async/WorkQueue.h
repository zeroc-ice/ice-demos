//
// Copyright (c) ZeroC, Inc. All rights reserved.
//

#ifndef WORK_QUEUE_H
#define WORK_QUEUE_H

#include <Hello.h>

#include <condition_variable>
#include <list>
#include <mutex>
#include <thread>

class WorkQueue
{
public:
    void run();

    void add(int, std::function<void()>, std::function<void(std::exception_ptr)>);
    void destroy();
    void start();
    void join();

private:
    using CallbackEntry = std::tuple<int, std::function<void()>, std::function<void(std::exception_ptr)>>;

    std::mutex _mutex;
    std::condition_variable _condition;

    std::list<CallbackEntry> _callbacks;
    bool _done{false};
    std::thread _thread;
};

#endif
