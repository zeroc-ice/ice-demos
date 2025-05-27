// Copyright (c) ZeroC, Inc.

#include "Chatbot.h"

#include <chrono>
#include <future>
#include <iostream>

using namespace std;

int
main(int argc, char* argv[])
{
    // CtrlCHandler is a helper class that handles Ctrl+C and similar signals. It must be constructed at the beginning
    // of the program, before creating an Ice communicator or starting any thread.
    Ice::CtrlCHandler ctrlCHandler;

    // Set the maximum number of threads in the server thread pool to 3, since Chatbot::greet waits synchronously.
    Ice::InitializationData initData;
    initData.properties = Ice::createProperties(argc, argv);
    initData.properties->setProperty("Ice.ThreadPool.Server.SizeMax", "3");

    // Create an Ice communicator. We'll use this communicator to create an object adapter.
    Ice::CommunicatorPtr communicator = Ice::initialize(initData);

    // Make sure the communicator is destroyed at the end of this scope.
    Ice::CommunicatorHolder communicatorHolder{communicator};

    // Create an object adapter that listens for incoming requests and dispatches them to servants.
    auto adapter = communicator->createObjectAdapterWithEndpoints("GreeterAdapter", "tcp -p 4061");

    // Create a promise to cancel outstanding dispatches after the user presses Ctrl+C.
    auto cancelDispatchPromise = make_shared<promise<void>>();
    shared_future<void> cancelDispatch{cancelDispatchPromise->get_future()};

    // Register two instances of Chatbot - a regular greater and a slow greeter.
    adapter->add(make_shared<Server::Chatbot>(), Ice::Identity{"greeter"});
    adapter->add(make_shared<Server::Chatbot>(60s, cancelDispatch), Ice::Identity{"slowGreeter"});

    // Start dispatching requests.
    adapter->activate();
    cout << "Listening on port 4061..." << endl;

    // Shut down the communicator when the user presses Ctrl+C.
    ctrlCHandler.setCallback(
        [communicator, cancelDispatchPromise, &ctrlCHandler](int signal)
        {
            cout << "Caught signal " << signal << ", shutting down..." << endl;
            communicator->shutdown();

            // Cancel outstanding dispatches "stuck" in the slow greeter.
            cancelDispatchPromise->set_value();

            // Clear the callback. Otherwise, a second Ctrl+C would call set_value again.
            ctrlCHandler.setCallback(nullptr);
        });

    // Wait until the communicator is shut down. Here, this occurs when the user presses Ctrl+C.
    communicator->waitForShutdown();

    return 0;
}
