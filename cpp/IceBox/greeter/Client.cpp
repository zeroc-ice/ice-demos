// Copyright (c) ZeroC, Inc.

#include "Greeter.h"

#include <cstdlib>
#include <iostream>

using namespace std;

int
main(int argc, char* argv[])
{
    // Figure out my name.
#ifdef _MSC_VER
    size_t requestedSize;
    char buffer[256];
    getenv_s(&requestedSize, buffer, sizeof(buffer), "USERNAME");
    const char* name = requestedSize > 0 ? buffer : nullptr;
#else
    const char* name = getenv("USER");
#endif
    if (name == nullptr)
    {
        name = "masked user";
    }

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
    string greeting = greeter->greet(name);
    cout << greeting << endl;

    return 0;
}
