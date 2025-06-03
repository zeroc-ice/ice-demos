// Copyright (c) ZeroC, Inc.

#include "ServerRun.h"
#include "Chatbot.h"
#include <Ice/Ice.h>
#include <iostream>

using namespace std;

void
Server::run(int argc, char* argv[], Ice::SSL::ServerAuthenticationOptions serverAuthenticationOptions)
{
    // CtrlCHandler is a helper class that handles Ctrl+C and similar signals. It must be constructed at the beginning
    // of the program, before creating an Ice communicator or starting any thread.
    Ice::CtrlCHandler ctrlCHandler;

    // Create an Ice communicator. We'll use this communicator to create an object adapter.
    Ice::CommunicatorPtr communicator = Ice::initialize(argc, argv);

    // Make sure the communicator is destroyed at the end of this scope.
    Ice::CommunicatorHolder communicatorHolder{communicator};

    // Create an object adapter that listens for incoming requests and dispatches them to servants.
    // The "ssl" endpoint ensures the server uses the ssl transport and only accepts secure connections.
    // The provided server authentication options are used to configure the ssl transport.
    auto adapter = communicator->createObjectAdapterWithEndpoints(
        "GreeterAdapter",
        "ssl -h localhost -p 4061",
        std::move(serverAuthenticationOptions));

    // Register the Chatbot servant with the adapter.
    adapter->add(make_shared<Server::Chatbot>(), Ice::Identity{"greeter"});

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
}
