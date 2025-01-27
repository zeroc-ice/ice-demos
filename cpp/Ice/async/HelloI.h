// Copyright (c) ZeroC, Inc.

#ifndef HELLO_I_H
#define HELLO_I_H

#include <Hello.h>
#include <WorkQueue.h>

class HelloI : public Demo::Hello
{
public:
    HelloI(const std::shared_ptr<WorkQueue>&);
    void
    sayHelloAsync(int, std::function<void()>, std::function<void(std::exception_ptr)>, const Ice::Current&) override;
    void shutdown(const Ice::Current&) override;

private:
    std::shared_ptr<WorkQueue> _workQueue;
};

#endif
