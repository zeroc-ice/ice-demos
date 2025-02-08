// Copyright (c) ZeroC, Inc.

#include "Chatbot.h"

#include <Ice/Ice.h>
#include <iostream>

using namespace std;
using namespace ConfigServer;

int
main(int argc, char* argv[])
{
    // CtrlCHandler is a helper class that handles Ctrl+C and similar signals. It must be constructed at the beginning
    // of the program, before creating an Ice communicator or starting any thread.
    Ice::CtrlCHandler ctrlCHandler;

    // Create an Ice communicator to initialize the Ice runtime. The communicator gets its configuration properties from
    // file "config.server". The communicator initialization also parses the command-line options to find and set
    // additional properties.
    const Ice::CommunicatorHolder communicatorHolder{argc, argv, "config.server"};
    const Ice::CommunicatorPtr& communicator = communicatorHolder.communicator();

    // Create an object adapter that listens for incoming requests and dispatches them to servants.
    // This adapter is configured through the GreeterAdapter.* properties in config.server.
    auto adapter = communicator->createObjectAdapter("GreeterAdapter");

    // Register the Chatbot servant with the adapter.
    adapter->add(make_shared<Chatbot>(), Ice::stringToIdentity("greeter"));

    // Start dispatching requests.
    adapter->activate();

    // Ice.Trace.Network=1 or greater shows on which interface(s) and port(s) the server is listening.

    // Wait until the user presses Ctrl+C.
    int signal = ctrlCHandler.wait();
    cout << "Caught signal " << signal << ", exiting..." << endl;

    return 0;
}
