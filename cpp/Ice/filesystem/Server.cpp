// Copyright (c) ZeroC, Inc.

#include "Memory.h"

#include <Ice/Ice.h>
#include <iostream>

using namespace std;

int
main(int argc, char* argv[])
{
    // CtrlCHandler is a helper class that handles Ctrl+C and similar signals. It must be constructed at the beginning
    // of the program, before creating an Ice communicator or starting any thread.
    Ice::CtrlCHandler ctrlCHandler;

    // Create an Ice communicator to initialize the Ice runtime. The CommunicatorHolder is a RAII helper that creates
    // the communicator in its constructor and destroys it when it goes out of scope.
    const Ice::CommunicatorHolder communicatorHolder{argc, argv};
    const Ice::CommunicatorPtr& communicator = communicatorHolder.communicator();

    // Create an object adapter that listens for incoming requests and dispatches them to servants.
    auto adapter = communicator->createObjectAdapterWithEndpoints("Filesystem", "tcp -p 4061");

    // Create the root directory servant (with name "/"), and add this servant to the adapter.
    auto root = make_shared<Server::MDirectory>("/");
    adapter->add(root, Ice::stringToIdentity("RootDir"));

    // Create a file called "README", add this servant to the adapter, and add the corresponding proxy to the root
    // directory.
    auto file = make_shared<Server::MFile>("README");
    file->writeDirect({"This file system contains a collection of poetry."});
    root->addChild(adapter->addWithUUID<Filesystem::FilePrx>(file));

    // Create a directory called "Coleridge", add this servant to the adapter, and add the corresponding proxy to the
    // root directory.
    auto coleridge = make_shared<Server::MDirectory>("Coleridge");
    root->addChild(adapter->addWithUUID<Filesystem::DirectoryPrx>(coleridge));

    // Create a file called "Kubla_Khan", add this servant to the adapter, and add the corresponding proxy to the
    // Coleridge directory.
    file = make_shared<Server::MFile>("Kubla_Khan");
    file->writeDirect(
        {"In Xanadu did Kubla Khan",
         "A stately pleasure-dome decree:",
         "Where Alph, the sacred river, ran",
         "Through caverns measureless to man",
         "Down to a sunless sea."});
    coleridge->addChild(adapter->addWithUUID<Filesystem::FilePrx>(file));

    // Start dispatching requests after registering all servants.
    adapter->activate();
    cout << "Listening on port 4061..." << endl;

    // Wait until the user presses Ctrl+C.
    int signal = ctrlCHandler.wait();
    cout << "Caught signal " << signal << ", exiting..." << endl;

    return 0;
}
