// Copyright (c) ZeroC, Inc.

#ifndef COUNTER_I_H
#define COUNTER_I_H

#include "Counter.h"
#include <IceStorm/IceStorm.h>
#include <mutex>

class CounterI final : public Demo::Counter
{
public:
    CounterI(const std::optional<IceStorm::TopicPrx>&);

    void subscribe(std::optional<Demo::CounterObserverPrx>, const Ice::Current&) final;
    void unsubscribe(std::optional<Demo::CounterObserverPrx>, const Ice::Current&) final;
    void inc(int value, const Ice::Current&) final;

private:
    std::mutex _mutex;
    int _value = 0;
    std::optional<IceStorm::TopicPrx> _topic;
    std::optional<Demo::CounterObserverPrx> _publisher;
};

#endif
