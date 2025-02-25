// Copyright (c) ZeroC, Inc.

#include "../../common/Env.h"
#include "Greeter.h"

#include <iostream>

using namespace std;

int
main(int argc, char* argv[])
{
    // Create an Ice communicator to initialize the Ice runtime. The communicator gets its configuration properties from
    // file config.client in the client's current working directory. The communicator initialization also parses the
    // command-line options to find and set additional properties.
    Ice::CommunicatorPtr communicator = Ice::initialize(argc, argv, "config.client");

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
