// Copyright (c) ZeroC, Inc.

#include "../../common/Env.h"
#include "Greeter.h"

#include <Ice/Ice.h>
#include <future>
#include <iostream>

using namespace std;

int
main(int argc, char* argv[])
{
    // Create an Ice communicator. We'll use this communicator to create proxies and manage outgoing connections.
    Ice::CommunicatorPtr communicator = Ice::initialize(argc, argv);

    // Make sure the communicator is destroyed at the end of this scope.
    Ice::CommunicatorHolder communicatorHolder{communicator};

    // GreeterPrx is a class generated by the Slice compiler. We create a proxy from a communicator and a "stringified
    // proxy" with the address of the target object.
    // If you run the server on a different computer, replace localhost in the string below with the server's hostname
    // or IP address.
    VisitorCenter::GreeterPrx greeter{communicator, "greeter:tcp -h localhost -p 4061"};

    // Send a request to the remote object and wait synchronously for the response.
    // Both the -> and . syntax can be used to make invocations with the proxy.
    string greeting = greeter->greet(Env::getUsername());
    cout << greeting << endl;

    // Send another request to the remote object, this time with greetAsync. greetAsync returns a future immediately.
    future<string> futureGreeting = greeter->greetAsync("alice");

    // Wait for the response.
    greeting = futureGreeting.get();
    cout << greeting << endl;

    // Send a third request to the remote object, this time with the greetAsync overload that accepts callbacks.
    promise<void> promise;
    greeter->greetAsync(
        "bob",
        [&promise](string_view greeting) // response callback
        {
            cout << greeting << endl;
            promise.set_value();
        },
        [&promise](std::exception_ptr exceptionPtr) // exception callback
        { promise.set_exception(exceptionPtr); });

    // Wait for the response/exception callback to be called.
    promise.get_future().get();

    return 0;
}
