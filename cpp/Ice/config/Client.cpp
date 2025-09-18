// Copyright (c) ZeroC, Inc.

#include "../../common/Env.h"
#include "Greeter.h"

#include <iostream>

using namespace std;

int
main(int argc, char* argv[])
{
    // Load the contents of the config.client file into a Properties object.
    auto configFileProperties = Ice::createProperties();
    configFileProperties->load("config.client");

    // Create a Properties object from the command line arguments and the config file properties; Ice.* properties and
    // other reserved properties set in argc/argv augment or override the config file properties.
    auto properties = Ice::createProperties(argc, argv, configFileProperties);

    // Create an Ice communicator. We'll use this communicator to create proxies and manage outgoing connections.
    // The communicator gets its properties from initData.properties.
    Ice::InitializationData initData;
    initData.properties = properties;
    Ice::CommunicatorPtr communicator = Ice::initialize(initData);

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
