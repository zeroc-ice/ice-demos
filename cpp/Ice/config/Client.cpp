// Copyright (c) ZeroC, Inc.

#include "../../common/Env.h"
#include "Greeter.h"

#include <iostream>

using namespace std;

int
main(int argc, char* argv[])
{
    // Create Ice properties from the contents of the config.client file in the current working directory.
    Ice::InitializationData initData;
    initData.properties = Ice::createProperties();
    initData.properties->load("config.client");

    // Create an Ice communicator. We'll use this communicator to create proxies and manage outgoing connections.
    // The communicator gets its properties from initData.properties; Ice.* properties and other reserved properties
    // set in argc/argv override these properties.
    Ice::CommunicatorPtr communicator = Ice::initialize(argc, argv, initData);

    // Make sure the communicator is destroyed at the end of this scope.
    Ice::CommunicatorHolder communicatorHolder{communicator};

    // We create a Greeter proxy using the value of the Greeter.Proxy property in config.client.
    // It's nullopt if the property is not set.
    std::optional<VisitorCenter::GreeterPrx> greeter =
        communicator->propertyToProxy<VisitorCenter::GreeterPrx>("Greeter.Proxy");

    // Send a request to the remote object and get the response.
    string greeting = greeter->greet(Env::getUsername());

    cout << greeting << endl;
    return 0;
}
