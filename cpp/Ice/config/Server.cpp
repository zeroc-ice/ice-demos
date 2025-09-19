// Copyright (c) ZeroC, Inc.

#include "Chatbot.h"

#include <Ice/Ice.h>
#include <iostream>

using namespace std;

int
main(int argc, char* argv[])
{
    // Create an Ice communicator. We'll use this communicator to create an object adapter. The communicator gets its
    // configuration properties from file config.server, in the server's current working directory. The communicator
    // initialization also parses the command-line options to find and set additional properties.
    Ice::CtrlCHandler ctrlCHandler;

    // Create Ice properties from the contents of the config.server file in the current working directory.
    auto configFileProperties = Ice::createProperties();
    configFileProperties->load("config.server");

    // Create a Properties object from the command line arguments and the config file properties; Ice.* properties and
    // other reserved properties set in argc/argv override the config file properties.
    auto properties = Ice::createProperties(argc, argv, configFileProperties);

    // Create an Ice communicator. We'll use this communicator to create an object adapter.
    // The communicator gets its properties from the properties object.
    Ice::InitializationData initData;
    initData.properties = properties;
    Ice::CommunicatorPtr communicator = Ice::initialize(initData);

    // Make sure the communicator is destroyed at the end of this scope.
    Ice::CommunicatorHolder communicatorHolder{communicator};

    // Create an object adapter that listens for incoming requests and dispatches them to servants.
    // This adapter is configured through the GreeterAdapter.* properties in config.server.
    auto adapter = communicator->createObjectAdapter("GreeterAdapter");

    // Register the Chatbot servant with the adapter.
    adapter->add(make_shared<Server::Chatbot>(), Ice::Identity{"greeter"});

    // Start dispatching requests.
    adapter->activate();

    // Ice.Trace.Network=1 or greater shows on which interface(s) and port(s) the server is listening.

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
