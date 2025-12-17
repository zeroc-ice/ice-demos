// Copyright (c) ZeroC, Inc.

#include "../../common/Env.h"
#include "../../common/Terminate.h"
#include "Greeter.h"

#include <Ice/Ice.h>
#include <iostream>

using namespace std;

int
main(int argc, char* argv[])
{
    // Set a custom terminate handler to print unhandled exceptions to cerr.
    std::set_terminate(Terminate::printCurrentException);

    // Create an Ice communicator. We'll use this communicator to create proxies and manage outgoing connections.
    Ice::CommunicatorPtr communicator = Ice::initialize(argc, argv);

    // Make sure the communicator is destroyed at the end of this scope.
    Ice::CommunicatorHolder communicatorHolder{communicator};

    // Set the default locator of the new communicator. It's the address of the Locator hosted by our IceGrid registry.
    // You can also set this proxy with the Ice.Default.Locator property.
    communicator->setDefaultLocator(Ice::LocatorPrx{communicator, "IceGrid/Locator:default -h localhost -p 4061"});

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
