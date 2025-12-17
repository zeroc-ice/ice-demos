// Copyright (c) ZeroC, Inc.

#include "../../common/Env.h"
#include "../../common/Terminate.h"
#include "../../common/Time.h"
#include "MockAlarmClock.h"

#include <Glacier2/Glacier2.h>
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
    // callbacks invoked through the Glacier2 router.
    string clientCategory = router->getCategoryForClient();

    // Create an object adapter with no name and no configuration, but with our router proxy. This object adapter is a
    // "bidirectional" object adapter, like the one created by the Ice/bidir client application. It does not listen on
    // any port and it does not need to be activated.
    Ice::ObjectAdapterPtr adapter = communicator->createObjectAdapterWithRouter("", router);

    adapter->activate();

    // Register the MockAlarmClock servant with the adapter. It uses the category retrieved from the router. You can
    // verify the Ring callback is never delivered if you provide a different category.
    auto mockAlarmClock = make_shared<Client::MockAlarmClock>();
    auto alarmClock = adapter->add<AlarmClockPrx>(mockAlarmClock, {"alarmClock", clientCategory});

    // Create a proxy to the wake-up service.
    WakeUpServicePrx wakeUpService{communicator, "wakeUpService:tcp -h localhost -p 4061"};

    // Configure the proxy to route requests using the Glacier2 router.
    wakeUpService = wakeUpService->ice_router(router);

    // Schedule a wake-up call in 5 seconds.
    wakeUpService->wakeMeUp(alarmClock, Time::toTimeStamp(chrono::system_clock::now() + 5s));
    cout << "Wake-up call scheduled, falling asleep..." << endl;

    // Wait until the "stop" button is pressed on the mock alarm clock.
    mockAlarmClock->wait();
    cout << "Stop button pressed, exiting..." << endl;

    return 0;
}
