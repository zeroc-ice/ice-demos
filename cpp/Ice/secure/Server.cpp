// Copyright (c) ZeroC, Inc.

#include "Chatbot.h"

#include <iostream>

using namespace std;

int
main(int argc, char* argv[])
{
    // CtrlCHandler is a helper class that handles Ctrl+C and similar signals. It must be constructed at the beginning
    // of the program, before creating an Ice communicator or starting any thread.
    Ice::CtrlCHandler ctrlCHandler;

    // Create an Ice communicator to initialize the Ice runtime. The new communicator gets its properties from file
    // "config.server".
    const Ice::CommunicatorHolder communicatorHolder{argc, argv, "config.server"};
    const Ice::CommunicatorPtr& communicator = communicatorHolder.communicator();

    // Create an object adapter that listens for incoming requests and dispatches them to servants. We use the ssl
    // transport for secure communications.
    auto adapter = communicator->createObjectAdapterWithEndpoints("GreeterAdapter", "ssl -p 4061");

    // Register the Chatbot servant with the adapter.
    adapter->add(make_shared<GreeterServer::Chatbot>(), Ice::stringToIdentity("greeter"));

    // Start dispatching requests.
    adapter->activate();

    // Wait until the user presses Ctrl+C.
    int signal = ctrlCHandler.wait();
    cout << "Caught signal " << signal << ", exiting..." << endl;

    return 0;
}
