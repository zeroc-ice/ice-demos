// Copyright (c) ZeroC, Inc.

#include "../../common/env.h"
#include "Greeter.h"

#include <cstdlib>
#include <iostream>

using namespace std;

int
main(int argc, char* argv[])
{
    // Create an Ice communicator to initialize the Ice runtime. The CommunicatorHolder is a RAII helper that creates
    // the communicator in its constructor and destroys it when it goes out of scope.
    const Ice::CommunicatorHolder communicatorHolder{argc, argv};
    const Ice::CommunicatorPtr& communicator = communicatorHolder.communicator();

    // GreeterPrx is a class generated by the Slice compiler. We create a proxy from a communicator and a "stringified
    // proxy" with the address of the target object.
    // If you run the server on a different computer, replace localhost in the string below with the server's hostname
    // or IP address.
    VisitorCenter::GreeterPrx greeter{communicator, "greeter:tcp -h localhost -p 4061"};

    // Send a request to the remote object and wait synchronously for the response.
    string greeting = greeter->greet(Env::getUsername());
    cout << greeting << endl;

    return 0;
}
