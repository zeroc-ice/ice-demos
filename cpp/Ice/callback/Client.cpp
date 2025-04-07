// Copyright (c) ZeroC, Inc.

#include "../../common/Time.h"
#include "MockAlarmClock.h"

#include <Ice/Ice.h>
#include <iostream>

using namespace EarlyRiser;
using namespace std;

int
main(int argc, char* argv[])
{
    // Create an Ice communicator. We'll use this communicator to create proxies and manage outgoing connections.
    Ice::CommunicatorPtr communicator = Ice::initialize(argc, argv);

    // Make sure the communicator is destroyed at the end of this scope.
    Ice::CommunicatorHolder communicatorHolder{communicator};

    // Create an object adapter that listens for incoming requests and dispatches them to servants.
    // Since we don't specify a port, the OS will choose an ephemeral port. This allows multiple client applications to
    // run concurrently on the same host.
    Ice::ObjectAdapterPtr adapter = communicator->createObjectAdapterWithEndpoints("AlarmClockAdapter", "tcp");

    // Register the MockAlarmClock servant with the adapter, and get an alarm clock proxy.
    auto mockAlarmClock = make_shared<Client::MockAlarmClock>();
    auto alarmClock = adapter->add<AlarmClockPrx>(mockAlarmClock, Ice::stringToIdentity("alarmClock"));

    // Start dispatching requests.
    adapter->activate();
    cout << "Listening on ephemeral port..." << endl;

    // Create a proxy to the wake-up service.
    WakeUpServicePrx wakeUpService{communicator, "wakeUpService:tcp -h localhost -p 4061"};

    // Schedule a wake-up call in 5 seconds.
    wakeUpService->wakeMeUp(alarmClock, Time::toTimeStamp(chrono::system_clock::now() + 5s));
    cout << "Wake-up call scheduled, falling asleep..." << endl;

    // Wait until the "stop" button is pressed on the mock alarm clock.
    mockAlarmClock->wait();
    cout << "Stop button pressed, exiting..." << endl;

    return 0;
}
