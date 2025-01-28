// Copyright (c) ZeroC, Inc.

#ifndef COUNTER_OBSERVER_I
#define COUNTER_OBSERVER_I

#include <Counter.h>
#include <mutex>

void print(const std::string&);

class CounterObserverI : public Demo::CounterObserver
{
public:
    void init(int, const Ice::Current&) override;
    void inc(int, const Ice::Current&) override;

private:
    int _value = 0;
    std::mutex _mutex;
};

#endif
