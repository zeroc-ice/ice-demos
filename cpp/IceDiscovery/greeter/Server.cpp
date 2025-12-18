// Copyright (c) ZeroC, Inc.

#include "../../common/Terminate.h"
#include "Chatbot.h"

#include <Ice/Ice.h>
#include <IceDiscovery/IceDiscovery.h>
#include <iostream>

using namespace std;

int
main(int argc, char* argv[])
{
    // Set a custom terminate handler to print unhandled exceptions to cerr.
    std::set_terminate(Terminate::printCurrentException);

    // Register the IceDiscovery plug-in. The plug-in will be loaded during communicator initialization.
    Ice::InitializationData initData;
    initData.properties = make_shared<Ice::Properties>(argc, argv);
    initData.pluginFactories = {IceDiscovery::discoveryPluginFactory()};

    // CtrlCHandler is a helper class that handles Ctrl+C and similar signals. It must be constructed at the beginning
    // of the program, before creating an Ice communicator or starting any thread.
    Ice::CtrlCHandler ctrlCHandler;

    // Create an Ice communicator. We'll use this communicator to create an object adapter.
    Ice::CommunicatorPtr communicator = Ice::initialize(initData);

    // Make sure the communicator is destroyed at the end of this scope.
    Ice::CommunicatorHolder communicatorHolder{communicator};

    // Get the communicator's properties. We'll use this object to set the properties of the object adapter.
    Ice::PropertiesPtr properties = communicator->getProperties();

    // Configure the object adapter GreeterAdapter. It must be an indirect object adapter (i.e., with an AdapterId
    // property); otherwise, the IceDiscovery plug-in can't make it discoverable by IceDiscovery clients.
    properties->setProperty("GreeterAdapter.AdapterId", "greeterAdapterId");

    // Configure the GreeterAdapter to listen on TCP with an OS-assigned port. We don't need a fixed port since the
    // clients discover this object adapter.
    properties->setProperty("GreeterAdapter.Endpoints", "tcp");

    // Create an object adapter that listens for incoming requests and dispatches them to servants.
    auto adapter = communicator->createObjectAdapter("GreeterAdapter");

    // Create an object adapter that listens for incoming requests and dispatches them to servants. "GreeterAdapter" is
    // a key into the configuration properties set above.
    adapter->add(make_shared<Server::Chatbot>(), Ice::Identity{"greeter"});

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
