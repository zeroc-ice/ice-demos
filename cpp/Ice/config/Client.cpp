// Copyright (c) ZeroC, Inc.

#include "Greeter.h"

#include <Ice/Ice.h>
#include <cstdlib>
#include <iostream>

using namespace std;

int
main(int argc, char* argv[])
{
    // Figure out my name.
    const char* name = getenv("USER");
    if (name == nullptr)
    {
        name = getenv("USERNAME");
    }
    if (name == nullptr)
    {
        name = "masked user";
    }

    // Create an Ice communicator to initialize the Ice runtime. The communicator gets its configuration properties from
    // file "config.server". The communicator initialization also parses the command-line options to find and set
    // additional properties.
    const Ice::CommunicatorHolder communicatorHolder{argc, argv, "config.client"};
    const Ice::CommunicatorPtr& communicator = communicatorHolder.communicator();

    // We create a Greeter proxy using the value of the "Greeter.Proxy" property in config.client.
    // It's nullopt if the property is not set.
    std::optional<VisitorCenter::GreeterPrx> greeter =
        communicator->propertyToProxy<VisitorCenter::GreeterPrx>("Greeter.Proxy");

    // Send a request to the remote object and get the response.
    string greeting = greeter->greet(name);

    cout << greeting << endl;
    return 0;
}
