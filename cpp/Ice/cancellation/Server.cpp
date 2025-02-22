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

    // Set the maximum number of threads in the server thread pool to 10, since Chatbot::greet waits synchronously.
    Ice::InitializationData initData;
    initData.properties = Ice::createProperties(argc, argv);
    initData.properties->setProperty("Ice.ThreadPool.Server.SizeMax", "10");

    // Create an Ice communicator to initialize the Ice runtime. The CommunicatorHolder is a RAII helper that creates
    // the communicator in its constructor and destroys it when it goes out of scope.
    const Ice::CommunicatorHolder communicatorHolder{initData};
    const Ice::CommunicatorPtr& communicator = communicatorHolder.communicator();

    // Create an object adapter that listens for incoming requests and dispatches them to servants.
    auto adapter = communicator->createObjectAdapterWithEndpoints("GreeterAdapter", "tcp -p 4061");

    // Create a promise to cancel outstanding dispatches after the user presses Ctrl+C.
    promise<void> cancelDispatchPromise;
    shared_future<void> cancelDispatch{cancelDispatchPromise.get_future()};

    // Register two instances of Chatbot - a regular greater and a slow greeter.
    adapter->add(make_shared<Server::Chatbot>(0s, cancelDispatch), Ice::stringToIdentity("greeter"));
    adapter->add(make_shared<Server::Chatbot>(60s, cancelDispatch), Ice::stringToIdentity("slowGreeter"));

    // Start dispatching requests.
    adapter->activate();
    cout << "Listening on port 4061..." << endl;

    // Wait until the user presses Ctrl+C.
    int signal = ctrlCHandler.wait();
    cout << "Caught signal " << signal << ", exiting..." << endl;

    return 0;
}
