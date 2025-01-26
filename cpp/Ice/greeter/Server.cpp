// Copyright (c) ZeroC, Inc.

#include "Chatbot.h"

#include <iostream>

using namespace std;

int
main(int argc, char* argv[])
{
    // CtrlCHandler handles Ctrl+C and similar signals. It must be created at the beginning of the program, before
    // starting any thread.
    Ice::CtrlCHandler ctrlCHandler;

    // Create an Ice communicator to initialize the Ice runtime. The CommunicatorHolder is a RAII helper that creates
    // the communicator in its constructor and destroys it when it goes out of scope.
    const Ice::CommunicatorHolder communicatorHolder{argc, argv};
    const Ice::CommunicatorPtr& communicator = communicatorHolder.communicator();

    ctrlCHandler.setCallback(
        [communicator](int signal) // TODO: capturing the communicator by const reference is not a good model.
        {
            cout << "Caught signal " << signal << ", shutting down... " << flush;
            communicator->shutdown(); // initiates the shutdown of the communicator
        });

    // Create an object adapter that listens for incoming requests and dispatches them to servants.
    // TODO: should we include a -h in this endpoint?
    auto adapter = communicator->createObjectAdapterWithEndpoints("GreeterAdapter", "tcp -p 4061");

    // Register the Chatbot servant with the adapter.
    adapter->add(make_shared<GreeterServer::Chatbot>(), Ice::stringToIdentity("greeter"));

    // Start dispatching requests.
    adapter->activate();

    // Wait until the application shuts down the communicator by pressing Ctrl+C.
    communicator->waitForShutdown();

    cout << "done" << endl;
    return 0;
}
