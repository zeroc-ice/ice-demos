// Copyright (c) ZeroC, Inc.

#include "../../common/Env.h"
#include "Greeter.h"

#include <Ice/Ice.h>
#include <iostream>

using namespace std;

int
main(int argc, char* argv[])
{
    // Create an Ice communicator to initialize the Ice runtime.
    Ice::CommunicatorPtr communicator = Ice::initialize(argc, argv);

    // Make sure the communicator is destroyed at the end of this scope.
    Ice::CommunicatorHolder communicatorHolder{communicator};

    // We create a Greeter proxy for a Greeter object in the Forwarder server (port 10000).
    VisitorCenter::GreeterPrx greeter{communicator, "greeter:tcp -h localhost -p 10000"};

    // Send a request to the remote object and wait for the response.
    string greeting = greeter->greet(Env::getUsername());
    cout << greeting << endl;

    return 0;
}
