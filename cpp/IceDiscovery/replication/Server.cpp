// Copyright (c) ZeroC, Inc.

#include "Chatbot.h"

#include <Ice/Ice.h>
#include <IceDiscovery/IceDiscovery.h>
#include <iostream>

using namespace std;

int
main(int argc, char* argv[])
{
    // Register the IceDiscovery plugin. The plugin will be loaded during communicator initialization.
    Ice::InitializationData initData;
    initData.properties = Ice::createProperties(argc, argv);
    initData.pluginFactories = {IceDiscovery::discoveryPluginFactory()};

    // CtrlCHandler is a helper class that handles Ctrl+C and similar signals. It must be constructed at the beginning
    // of the program, before creating an Ice communicator or starting any thread.
    Ice::CtrlCHandler ctrlCHandler;

    // Create an Ice communicator. We'll use this communicator to create an object adapter.
    Ice::CommunicatorPtr communicator = Ice::initialize(argc, argv);

    // Make sure the communicator is destroyed at the end of this scope.
    Ice::CommunicatorHolder communicatorHolder{communicator};

    // Get the communicator's properties. We'll use this object to set the properties of the object adapter.
    Ice::PropertiesPtr properties = communicator->getProperties();

    // Generate a unique name for the adapter ID and the greeter name.
    string uuid = Ice::generateUUID();

    // Configure the object adapter GreeterAdapter. It must be an indirect object adapter (i.e., with an AdapterId
    // property); otherwise, the IceDiscovery plugin can't make it discoverable by IceDiscovery clients.
    // We also set the ReplicaGroupId property to "greeterPool" to enable replication.
    properties->setProperty("GreeterAdapter.AdapterId", "greeter-" + uuid);
    properties->setProperty("GreeterAdapter.ReplicaGroupId", "greeterPool");

    // Configure the GreeterAdapter to listen on TCP with an OS-assigned port. We don't need a fixed port since the
    // clients discover this object adapter.
    properties->setProperty("GreeterAdapter.Endpoints", "tcp");

    // Create an object adapter that listens for incoming requests and dispatches them to servants.
    auto adapter = communicator->createObjectAdapter("GreeterAdapter");

    // Register the Chatbot servant with the adapter. Here, well-known object "greeter" is replicated across all server
    // instances.
    adapter->add(make_shared<Server::Chatbot>(uuid.substr(uuid.length() - 4)), Ice::stringToIdentity("greeter"));

    // Start dispatching requests.
    adapter->activate();
    cout << "Listening..." << endl;

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
