// Copyright (c) ZeroC, Inc.

#include "../../common/Env.h"
#include "Greeter.h"

#include <Ice/Ice.h>
#include <IceLocatorDiscovery/IceLocatorDiscovery.h>
#include <iostream>

using namespace std;

int
main(int argc, char* argv[])
{
    // Configure the communicator to load the IceLocatorDiscovery plug-in during initialization.
    // This plug-in will discover the locator (IceGrid registry in this demo) to use. As a result, we don't need to
    // configure the default locator on this communicator.
    Ice::InitializationData initData;
    initData.properties = Ice::createProperties(argc, argv);
    initData.pluginFactories = {IceLocatorDiscovery::locatorDiscoveryPluginFactory()};

    // Create an Ice communicator. We'll use this communicator to create proxies and manage outgoing connections.
    Ice::CommunicatorPtr communicator = Ice::initialize(initData);

    // Make sure the communicator is destroyed at the end of this scope.
    Ice::CommunicatorHolder communicatorHolder{communicator};

    // Create a proxy to the Greeter object hosted by the server. "greeter" is a stringified proxy with no addressing
    // information, also known as a well-known proxy. It's specified by the <object> element in the IceGrid XML file.
    // The IceGrid registry resolves this well-known proxy and returns the actual address (endpoint) of the server to
    // this client.
    VisitorCenter::GreeterPrx greeter{communicator, "greeter"};

    // Send a request to the remote object and get the response.
    string greeting = greeter->greet(Env::getUsername());
    cout << greeting << endl;

    // Send another request to the remote object. With the default configuration we use for this client, this request
    // reuses the connection and reaches the same server, even when we have multiple replicated servers.
    greeting = greeter->greet("alice");
    cout << greeting << endl;

    return 0;
}
