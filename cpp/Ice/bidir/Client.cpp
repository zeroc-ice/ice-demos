// Copyright (c) ZeroC, Inc.

#include "../../common/Terminate.h"
#include "../../common/Time.h"
#include "MockAlarmClock.h"

#include <Ice/Ice.h>
#include <iostream>

using namespace EarlyRiser;
using namespace std;

int
main(int argc, char* argv[])
{
    // Set a custom terminate handler to print unhandled exceptions to cerr.
    std::set_terminate(Terminate::printCurrentException);

    // Create an Ice communicator. We'll use this communicator to create proxies, manage outgoing connections, and
    // create an object adapter.
    Ice::CommunicatorPtr communicator = Ice::initialize(argc, argv);

    // Make sure the communicator is destroyed at the end of this scope.
    Ice::CommunicatorHolder communicatorHolder{communicator};

    // Create an object adapter with no name and no configuration. This object adapter does not need to be activated.
    Ice::ObjectAdapterPtr adapter = communicator->createObjectAdapter("");

    // Sets this object adapter as the default object adapter on the communicator.
    communicator->setDefaultObjectAdapter(adapter);

    // Register the MockAlarmClock servant with the adapter. The wake up service knows we use identity "alarmClock".
    auto mockAlarmClock = make_shared<Client::MockAlarmClock>();
    adapter->add(mockAlarmClock, Ice::Identity{"alarmClock"});

    // Create a proxy to the wake-up service.
    WakeUpServicePrx wakeUpService{communicator, "wakeUpService:tcp -h localhost -p 4061"};

    // Schedule a wake-up call in 5 seconds.
    wakeUpService->wakeMeUp(Time::toTimeStamp(chrono::system_clock::now() + 5s));
    cout << "Wake-up call scheduled, falling asleep..." << endl;

    // Wait until the "stop" button is pressed on the mock alarm clock.
    mockAlarmClock->wait();
    cout << "Stop button pressed, exiting..." << endl;

    return 0;
}
