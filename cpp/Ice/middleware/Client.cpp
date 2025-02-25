// Copyright (c) ZeroC, Inc.

#include "../../common/Env.h"
#include "Greeter.h"

#include <iostream>

using namespace std;

int
main(int argc, char* argv[])
{
    // Create an Ice communicator to initialize the Ice runtime.
    Ice::CommunicatorPtr communicator = Ice::initialize(argc, argv);

    // Make sure the communicator is destroyed at the end of this scope.
    Ice::CommunicatorHolder communicatorHolder{communicator};

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
    catch (const Ice::DispatchException& dispatchException)
    {
        // We expect a dispatch exception with reply status Unauthorized for an invalid (or missing) token.
        // See AuthorizationMiddleware.
        if (dispatchException.replyStatus() != Ice::ReplyStatus::Unauthorized)
        {
            cout << "Received unexpected reply status: " << dispatchException.replyStatus() << endl;
        }
    }

    // Send a request with the correct token in the request context.
    string greeting = greeter.greet(Env::getUsername(), {{"token", validToken}});
    cout << greeting << endl;

    return 0;
}
