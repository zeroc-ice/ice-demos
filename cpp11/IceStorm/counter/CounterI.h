// **********************************************************************
//
// Copyright (c) 2003-present ZeroC, Inc. All rights reserved.
//
// **********************************************************************

#ifndef COUNTER_I_H
#define COUNTER_I_H

#include <IceStorm/IceStorm.h>
#include <Counter.h>
#include <mutex>

class CounterI : public Demo::Counter
{
public:

    CounterI(const std::shared_ptr<IceStorm::TopicPrx>&);

    virtual void subscribe(std::shared_ptr<Demo::CounterObserverPrx>, const Ice::Current&) override;
    virtual void unsubscribe(std::shared_ptr<Demo::CounterObserverPrx>, const Ice::Current&) override;
    virtual void inc(int value, const Ice::Current&) override;

private:

    std::mutex _mutex;
    int _value = 0;
    std::shared_ptr<IceStorm::TopicPrx> _topic;
    std::shared_ptr<Demo::CounterObserverPrx> _publisher;
};

#endif
