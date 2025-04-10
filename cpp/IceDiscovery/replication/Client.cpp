// Copyright (c) ZeroC, Inc.

#include "../../common/Env.h"
#include "Greeter.h"

#include <Ice/Ice.h>
#include <future>
#include <iostream>

using namespace std;

int
main(int argc, char* argv[])
{
    // Register the IceDiscovery plugin. The plugin will be loaded during communicator initialization.
    Ice::registerIceDiscovery();

    // Create an Ice communicator. We'll use this communicator to create proxies and manage outgoing connections.
    // The IceDiscovery plugin is created and initialized by initialize, and sets the default locator on the new
    // communicator.
    Ice::CommunicatorPtr communicator = Ice::initialize(argc, argv);

    // Make sure the communicator is destroyed at the end of this scope.
    Ice::CommunicatorHolder communicatorHolder{communicator};

    // Create a proxy to the Greeter object hosted by the server. "greeter" is a stringified proxy with no addressing
    // information, also known as a well-known proxy. It's resolved by the default locator installed by the IceDiscovery
    // plugin.
    VisitorCenter::GreeterPrx greeter{communicator, "greeter"};

    // Send a request to the remote object and wait synchronously for the response.
    // Both the -> and . syntax can be used to make invocations with the proxy.
    string greeting = greeter->greet(Env::getUsername());
    cout << greeting << endl;

    return 0;
}
