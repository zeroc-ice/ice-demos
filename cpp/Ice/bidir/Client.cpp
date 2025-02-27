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

    // Create an object adapter with no name and no configuration. This object adapter does not need to be activated.
    Ice::ObjectAdapterPtr adapter = communicator->createObjectAdapter("");

    // Sets this object adapter as the default object adapter on the communicator.
    communicator->setDefaultObjectAdapter(adapter);

    // Register the MockAlarmClock servant with the adapter. The wake up service knows we use identity "alarmClock".
    auto mockAlarmClock = make_shared<Client::MockAlarmClock>();
    adapter->add(mockAlarmClock, Ice::stringToIdentity("alarmClock"));

    // Create a proxy to the wake-up service.
    WakeUpServicePrx wakeUpService{communicator, "wakeUpService:tcp -h localhost -p 4061"};

    // Schedule a wake-up call in 5 seconds.
    wakeUpService->wakeMeUp(toTimeStamp(chrono::system_clock::now() + chrono::seconds{5}));
    cout << "Wake-up call scheduled, falling asleep..." << endl;

    // Wait until the "stop" button is pressed on the mock alarm clock.
    mockAlarmClock->wait();
    cout << "Stop button pressed, exiting..." << endl;

    return 0;
}
