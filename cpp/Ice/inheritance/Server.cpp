// Copyright (c) ZeroC, Inc.

#include "Memory.h"

#include <Ice/Ice.h>
#include <iostream>

using namespace std;

int
main(int argc, char* argv[])
{
    // CtrlCHandler is a helper class that handles Ctrl+C and similar signals.
    // It must be constructed at the beginning of the program,
    // before creating an Ice communicator or starting any thread.
    Ice::CtrlCHandler ctrlCHandler;

    // Create an Ice communicator. We'll use this communicator to create an object adapter.
    Ice::CommunicatorPtr communicator = Ice::initialize(argc, argv);

    // Make sure the communicator is destroyed at the end of this scope.
    Ice::CommunicatorHolder communicatorHolder{communicator};

    // Create an object adapter that listens for incoming requests and dispatches them to servants.
    auto adapter = communicator->createObjectAdapterWithEndpoints("Inheritance", "tcp -p 4061");

    // Create the root directory servant (with name "/"), and add this servant to the adapter.
    auto root = make_shared<Server::MDirectory>("/");
    adapter->add(root, Ice::Identity{"RootDir"});

    // Create a file called "README", add this servant to the adapter,
    // and add the corresponding proxy to the root directory.
    auto file = make_shared<Server::MFile>("README");
    file->writeDirect({"This file system contains a collection of poetry."});
    root->addChild(adapter->addWithUUID<Inheritance::FilePrx>(file));

    // Create a directory called "Coleridge", add this servant to the adapter,
    // and add the corresponding proxy to the root directory.
    auto coleridge = make_shared<Server::MDirectory>("Coleridge");
    root->addChild(adapter->addWithUUID<Inheritance::DirectoryPrx>(coleridge));

    // Create a file called "Kubla_Khan", add this servant to the adapter,
    // and add the corresponding proxy to the Coleridge directory.
    file = make_shared<Server::MFile>("Kubla_Khan");
    file->writeDirect(
        {"In Xanadu did Kubla Khan",
         "A stately pleasure-dome decree:",
         "Where Alph, the sacred river, ran",
         "Through caverns measureless to man",
         "Down to a sunless sea."});
    coleridge->addChild(adapter->addWithUUID<Inheritance::FilePrx>(file));

    // Start dispatching requests after registering all servants.
    adapter->activate();
    cout << "Listening on port 4061..." << endl;

    // Shut down the communicator when the user presses Ctrl+C.
    ctrlCHandler.setCallback(
        [communicator](int signal)
        {
            cout << "Caught signal " << signal << ", shutting down..." << endl;
            communicator->shutdown();
        });

    // Wait until the communicator is shut down.
    // Here, this occurs when the user presses Ctrl+C.
    communicator->waitForShutdown();

    return 0;
}
