// Copyright (c) ZeroC, Inc.

#include "../../common/Env.h"
#include "Greeter.h"

#include <Ice/Ice.h>
#include <IceGrid/IceGrid.h>
#include <iostream>

using namespace std;

int
main(int argc, char* argv[])
{
    // Create an Ice communicator. We'll use this communicator to create proxies and manage outgoing connections.
    Ice::CommunicatorPtr communicator = Ice::initialize(argc, argv);

    // Make sure the communicator is destroyed at the end of this scope.
    Ice::CommunicatorHolder communicatorHolder{communicator};

    // Set the default locator of the new communicator. It's the address of the Locator hosted by our IceGrid registry.
    // You can also set this proxy with the Ice.Default.Locator property.
    communicator->setDefaultLocator(Ice::LocatorPrx{communicator, "IceGrid/Locator:default -h localhost -p 4061"});

    // Create a proxy to the Query object hosted by the IceGrid registry. "IceGrid/Query" a well-known proxy, without
    // addressing information.
    IceGrid::QueryPrx query{communicator, "IceGrid/Query"};

    // Look up an object with type ::VisitorCenter::Greeter.
    string greeterTypeId = VisitorCenter::Greeter::ice_staticId(); // ::VisitorCenter::Greeter
    optional<Ice::ObjectPrx> proxy = query->findObjectByType(greeterTypeId);

    if (!proxy)
    {
        cout << "The IceGrid registry doesn't know any object with type '" << greeterTypeId << "'." << endl;
        return 0;
    }

    // Cast the object proxy to a Greeter proxy.
    auto greeter = Ice::uncheckedCast<VisitorCenter::GreeterPrx>(*proxy);

    // Send a request to the remote object and get the response.
    string greeting = greeter->greet(Env::getUsername());
    cout << greeting << endl;

    // Send another request to the remote object. With the default configuration we use for this client, this request
    // reuses the connection and reaches the same server.
    greeting = greeter->greet("alice");
    cout << greeting << endl;

    return 0;
}
