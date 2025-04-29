// Copyright (c) ZeroC, Inc.

#include "GreeterService.h"
#include "Chatbot.h"

#include <Ice/Ice.h>
#include <cassert>
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
    // "GreeterAdapter" must match the name of the adapter in the IceGrid XML file.
    assert(!_adapter);
    _adapter = communicator->createObjectAdapter("GreeterAdapter");

    // Retrieve the greeter name and greeter identity from the IceGrid-generated config file
    Ice::PropertiesPtr properties = communicator->getProperties();
    _greeterName = properties->getProperty("Greeter.Name");
    Ice::Identity greeterIdentity = Ice::stringToIdentity(properties->getProperty("Greeter.Identity"));

    // Register the Chatbot servant with the adapter.
    _adapter->add(make_shared<Server::Chatbot>(_greeterName), greeterIdentity);

    // Start dispatching requests.
    _adapter->activate();

    cout << _greeterName << " is listening..." << endl;
}

void
Service::GreeterService::stop()
{
    cout << _greeterName << " is shutting down..." << endl;

    // We destroy the object adapter in stop() in case the service is started, stopped and then restarted again
    // programmatically or using an admin tool.
    // The general rule is stop should cleanup all resources created by start.
    assert(_adapter);
    _adapter->destroy();
    _adapter = nullptr;
}
