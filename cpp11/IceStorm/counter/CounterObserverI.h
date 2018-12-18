// **********************************************************************
//
// Copyright (c) 2003-present ZeroC, Inc. All rights reserved.
//
// **********************************************************************

#ifndef COUNTER_OBSERVER_I
#define COUNTER_OBSERVER_I

#include <Counter.h>
#include <mutex>

void print(const std::string&);

class CounterObserverI : public Demo::CounterObserver
{
public:

    virtual void init(int, const Ice::Current&) override;
    virtual void inc(int, const Ice::Current&) override;

private:

    int _value = 0;
    std::mutex _mutex;
};

#endif
