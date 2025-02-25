// Copyright (c) ZeroC, Inc.

#include "MockAlarmClock.h"

#include <Ice/Ice.h>
#include <iostream>

using namespace EarlyRiser;
using namespace std;

namespace
{
    // Converts a time point to a time stamp.
    int64_t toTimeStamp(const chrono::system_clock::time_point& timePoint)
    {
        const int daysBeforeEpoch = 719162;

        int64_t timeStampMicro =
            chrono::duration_cast<chrono::microseconds>(timePoint.time_since_epoch() + daysBeforeEpoch * 24h).count();

        // The time stamp is in ticks, where each tick is 100 nanoseconds = 0.1 microsecond.
        return timeStampMicro * 10;
    }
}

int
main(int argc, char* argv[])
{
    // Create an Ice communicator to initialize the Ice runtime.
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
    wakeUpService->wakeMeUp(alarmClock, toTimeStamp(chrono::system_clock::now() + chrono::seconds{5}));
    cout << "Wake-up call scheduled, falling asleep..." << endl;

    // Wait until the "stop" button is pressed on the mock alarm clock.
    mockAlarmClock->wait();
    cout << "Stop button pressed, exiting..." << endl;

    return 0;
}
