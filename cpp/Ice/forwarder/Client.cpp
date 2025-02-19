// Copyright (c) ZeroC, Inc.

#include "../../common/env.h"
#include "Greeter.h"

#include <cstdlib>
#include <iostream>

using namespace std;

int
main(int argc, char* argv[])
{
    // Create an Ice communicator to initialize the Ice runtime.
    const Ice::CommunicatorHolder communicatorHolder{argc, argv};
    const Ice::CommunicatorPtr& communicator = communicatorHolder.communicator();

    // We create a Greeter proxy for a Greeter object in the Forwarder server (port 10000).
    VisitorCenter::GreeterPrx greeter{communicator, "greeter:tcp -h localhost -p 10000"};

    // Send a request to the remote object and wait for the response.
    string greeting = greeter->greet(Env::getUsername());
    cout << greeting << endl;

    return 0;
}
