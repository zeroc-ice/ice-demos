// **********************************************************************
//
// Copyright (c) 2003-2016 ZeroC, Inc. All rights reserved.
//
// **********************************************************************

#ifndef COUNTER_I_H
#define COUNTER_I_H

#include <IceStorm/IceStorm.h>
#include <Counter.h>

class CounterI : public Demo::Counter, public IceUtil::Mutex
{
public:

    CounterI(const IceStorm::TopicPrx&);

    virtual void subscribe(const Demo::CounterObserverPrx& observer, const Ice::Current&);
    virtual void unsubscribe(const Demo::CounterObserverPrx& observer, const Ice::Current&);
    virtual void inc(int value, const Ice::Current&);

private:

    int _value;
    IceStorm::TopicPrx _topic;
    Demo::CounterObserverPrx _publisher;
};

#endif
