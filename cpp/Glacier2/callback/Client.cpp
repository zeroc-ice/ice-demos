// Copyright (c) ZeroC, Inc.

#include "../../common/Env.h"
#include "MockAlarmClock.h"

#include <Glacier2/Glacier2.h>
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

    // Create a proxy to the Glacier2 router. The addressing information (transport, host and port number) is derived
    // from the value of Glacier2.Client.Endpoints in the glacier2 router configuration file.
    Glacier2::RouterPrx router{communicator, "Glacier2/router:tcp -h localhost -p 4063"};

    // Create a session with the Glacier2 router. In this demo, the Glacier2 router is configured to accept any
    // username/password combination. This call establishes a network connection to the Glacier2 router; the lifetime of
    // the session is the same as the lifetime of the connection.
    optional<Glacier2::SessionPrx> session = router->createSession(Env::getUsername(), "password");

    // The proxy returned by createSession is null because we did not configure a SessionManager on the Glacier2 router.
    assert(!session);

    // Obtain a category string from the router. We need to use this category for the identity of server->client
    // callbacks
    // invoked through the Glacier2 router.
    string clientCategory = router.getCategoryForClient();

    // Create an object adapter with no name and no configuration, but with our router proxy. This object adapter is a
    // "bidirectional" object adapter, like the one created by the Ice/bidir client application. It does not listen on
    // any port and it does not need to be activated.
    Ice::ObjectAdapterPtr adapter = communicator->createObjectAdapterWithRouter("", router);

    adapter->activate();

    // Register the MockAlarmClock servant with the adapter. It uses the category retrieved from the router. You can
    // verify the Ring callback is never delivered if you provide a different category.
    auto mockAlarmClock = make_shared<Client::MockAlarmClock>();
    auto alarmClock = adapter->add<AlarmClockPrx>(mockAlarmClock, {.name = "alarmClock", .category = clientCategory});

    // Create a proxy to the wake-up service.
    WakeUpServicePrx wakeUpService{communicator, "wakeUpService:tcp -h localhost -p 4061"};

    // Configure the proxy to route requests using the Glacier2 router.
    wakeUpService = wakeUpService.ice_router(router);

    // Schedule a wake-up call in 5 seconds.
    wakeUpService->wakeMeUp(alarmClock, toTimeStamp(chrono::system_clock::now() + chrono::seconds{5}));
    cout << "Wake-up call scheduled, falling asleep..." << endl;

    // Wait until the "stop" button is pressed on the mock alarm clock.
    mockAlarmClock->wait();
    cout << "Stop button pressed, exiting..." << endl;

    return 0;
}
