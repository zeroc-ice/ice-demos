// Copyright (c) ZeroC, Inc.

#include "../../common/Env.h"
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

    // Create a Greeter proxy. If you run the server on a different computer, replace localhost in the string below with
    // the server's hostname or IP address.
    VisitorCenter::GreeterPrx greeter{communicator, "greeter:tcp -h localhost -p 4061"};

    // We hardcode the tokens in this demo, for simplicity. A real application would obtain the token from a secure
    // source.
    const string validToken = "iced tea";

    // Send a request to the remote object with an invalid token in the request context.
    try
    {
        string unexpected = greeter.greet(Env::getUsername(), {{"token", "pineapple"}});
        cout << "Received unexpected greeting: " << unexpected << endl;
    }
    catch (const Ice::ObjectNotExistException&)
    {
        // Expected with an invalid (or missing) token. See AuthorizationMiddleware.
    }

    // Send a request with the correct token in the request context.
    string greeting = greeter.greet(Env::getUsername(), {{"token", validToken}});
    cout << greeting << endl;

    return 0;
}
