// Copyright (c) ZeroC, Inc.

#include "Forwarder.h"

#include <Ice/Ice.h>
#include <iostream>

using namespace std;

int
main(int argc, char* argv[])
{
    // CtrlCHandler is a helper class that handles Ctrl+C and similar signals. It must be constructed at the beginning
    // of the program, before creating an Ice communicator or starting any thread.
    Ice::CtrlCHandler ctrlCHandler;

    // Create an Ice communicator. We'll use this communicator to create an object adapter, and to create proxies and
    // manage outgoing connections.
    Ice::CommunicatorPtr communicator = Ice::initialize(argc, argv);

    // Make sure the communicator is destroyed at the end of this scope.
    Ice::CommunicatorHolder communicatorHolder{communicator};

    // Create an object adapter that listens for incoming requests and dispatches them to servants.
    auto adapter = communicator->createObjectAdapterWithEndpoints("ForwarderAdapter", "tcp -p 10000");

    // Create a target proxy template, with a dummy identity.
    Ice::ObjectPrx targetTemplate{communicator, "dummy:tcp -h localhost -p 4061"};

    // Register the Forwarder servant as default servant with the object adapter. The empty category means this default
    // servant receives requests to all Ice objects.
    adapter->addDefaultServant(make_shared<ForwardingServer::Forwarder>(targetTemplate), "");

    // Start dispatching requests.
    adapter->activate();
    cout << "Listening on port 10000..." << endl;

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
