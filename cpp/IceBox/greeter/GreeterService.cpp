// Copyright (c) ZeroC, Inc.

#include "GreeterService.h"
#include "Chatbot.h"

#include <Ice/Ice.h>

#include <iostream>

using namespace std;

extern "C"
{
    // Create an instance of the Greeter service. IceBox is configured to call this extern "C" function at runtime.
    ICE_DECLSPEC_EXPORT IceBox::Service* create(const Ice::CommunicatorPtr&) { return new Service::GreeterService; }
}

void
Service::GreeterService::start(
    [[maybe_unused]] const string& name,
    const Ice::CommunicatorPtr& communicator,
    [[maybe_unused]] const Ice::StringSeq& args)
{
    // Create an object adapter that listens for incoming requests and dispatches them to servants.
    auto adapter = communicator->createObjectAdapterWithEndpoints("GreeterAdapter", "tcp -p 4061");

    // Register the Chatbot servant with the adapter.
    adapter->add(make_shared<GreeterServer::Chatbot>("Hal"), Ice::stringToIdentity("greeter"));

    // Start dispatching requests.
    adapter->activate();
    cout << "Listening on port 4061..." << endl;
}

void
Service::GreeterService::stop()
{
    // Nothing to do here.
}
