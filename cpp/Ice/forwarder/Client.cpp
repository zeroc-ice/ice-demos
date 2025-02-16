// Copyright (c) ZeroC, Inc.

#include "Greeter.h"

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

    // Create an Ice communicator to initialize the Ice runtime.
    const Ice::CommunicatorHolder communicatorHolder{argc, argv};
    const Ice::CommunicatorPtr& communicator = communicatorHolder.communicator();

    // We create a Greeter proxy for a Greeter object in the Forwarder server (port 10000).
    VisitorCenter::GreeterPrx greeter{communicator, "greeter:tcp -h localhost -p 10000"};

    // Send a request to the remote object and wait for the response.
    string greeting = greeter->greet(name);
    cout << greeting << endl;

    return 0;
}
