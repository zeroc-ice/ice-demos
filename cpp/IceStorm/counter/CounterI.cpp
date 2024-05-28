//
// Copyright (c) ZeroC, Inc. All rights reserved.
//

#include "CounterI.h"
#include <Ice/Ice.h>

using namespace std;
using namespace Demo;

CounterI::CounterI(const optional<IceStorm::TopicPrx>& topic)
    : _topic(topic),
      _publisher(Ice::uncheckedCast<CounterObserverPrx>(topic->getPublisher()))
{
}

void
CounterI::subscribe(optional<CounterObserverPrx> observer, const Ice::Current&)
{
    const lock_guard<mutex> sync(_mutex);

    //
    // Subscribe to the IceStorm topic. This returns a per-subscriber
    // object which is then used to send the initialize event to just
    // the given subscriber.
    //
    auto o = Ice::uncheckedCast<CounterObserverPrx>(_topic->subscribeAndGetPublisher(IceStorm::QoS(), observer));
    o->init(_value);
}

void
CounterI::unsubscribe(optional<CounterObserverPrx> observer, const Ice::Current&)
{
    _topic->unsubscribe(observer);
}

void
CounterI::inc(int value, const Ice::Current&)
{
    const lock_guard<mutex> sync(_mutex);

    _value += value;
    _publisher->inc(value);
}
