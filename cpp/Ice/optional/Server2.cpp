// Copyright (c) ZeroC, Inc.

#include "ConsolePrinter2.h"

#include <Ice/Ice.h>
#include <iostream>
#include <random>

using namespace ClearSky;
using namespace std;

int
main(int argc, char* argv[])
{
    // CtrlCHandler is a helper class that handles Ctrl+C and similar signals.
    Ice::CtrlCHandler ctrlCHandler;

    // Create an Ice communicator. We'll use this communicator to create an object adapter.
    Ice::CommunicatorPtr communicator = Ice::initialize(argc, argv);

    // Make sure the communicator is destroyed at the end of this scope.
    Ice::CommunicatorHolder communicatorHolder{communicator};

    // Create an object adapter that listens for incoming requests and dispatches them to servants.
    Ice::ObjectAdapterPtr adapter = communicator->createObjectAdapterWithEndpoints("StationAdapter", "tcp -p 4061");

    // Register the ConsolePrinter servant with the adapter.
    adapter->add(make_shared<Server::ConsolePrinter>(), Ice::Identity{"weatherStation"});

    // Start dispatching requests.
    adapter->activate();
    cout << "Listening on port 4061..." << endl;

    // Shut down the communicator when the user presses Ctrl+C.
    ctrlCHandler.setCallback(
        [communicator](int signal)
        {
            cout << "Caught signal " << signal << ", shutting down..." << endl;
            communicator->shutdown();
        });

    // Wait until the communicator is shut down. Here, this occurs when the user presses Ctrl+C.
    communicator->waitForShutdown();

    return 0;
}
