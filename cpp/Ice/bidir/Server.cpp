// Copyright (c) ZeroC, Inc.

#include "BidirWakeUpService.h"

#include <iostream>

using namespace std;

int
main(int argc, char* argv[])
{
    // CtrlCHandler is a helper class that handles Ctrl+C and similar signals.
    Ice::CtrlCHandler ctrlCHandler;

    // Create an Ice communicator to initialize the Ice runtime.
    const Ice::CommunicatorHolder communicatorHolder{argc, argv};
    const Ice::CommunicatorPtr& communicator = communicatorHolder.communicator();

    // Create an object adapter that listens for incoming requests and dispatches them to servants.
    auto adapter = communicator->createObjectAdapterWithEndpoints("WakeUpAdapter", "tcp -p 4061");

    // Register the BidirWakeUpService servant with the adapter.
    adapter->add(make_shared<Server::BidirWakeUpService>(), Ice::stringToIdentity("wakeUpService"));

    // Start dispatching requests.
    adapter->activate();
    cout << "Listening on port 4061..." << endl;

    // Wait until the user presses Ctrl+C.
    int signal = ctrlCHandler.wait();
    cout << "Caught signal " << signal << ", exiting..." << endl;

    return 0;
}
