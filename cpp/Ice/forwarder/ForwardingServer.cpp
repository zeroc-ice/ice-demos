// Copyright (c) ZeroC, Inc.

#include "Forwarder.h"

#include <iostream>

using namespace std;

int
main(int argc, char* argv[])
{
    // CtrlCHandler is a helper class that handles Ctrl+C and similar signals. It must be constructed at the beginning
    // of the program, before creating an Ice communicator or starting any thread.
    Ice::CtrlCHandler ctrlCHandler;

    // Create an Ice communicator to initialize the Ice runtime. The CommunicatorHolder is a RAII helper that creates
    // the communicator in its constructor and destroys it when it goes out of scope.
    const Ice::CommunicatorHolder communicatorHolder{argc, argv};
    const Ice::CommunicatorPtr& communicator = communicatorHolder.communicator();

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

    // Wait until the user presses Ctrl+C.
    int signal = ctrlCHandler.wait();
    cout << "Caught signal " << signal << ", exiting..." << endl;

    return 0;
}
