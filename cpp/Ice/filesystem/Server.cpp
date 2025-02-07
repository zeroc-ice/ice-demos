// Copyright (c) ZeroC, Inc.

#include "Memory.h"

#include <Ice/Ice.h>
#include <iostream>

using namespace std;
using namespace Memory;

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

    // Create the root directory servant (with name "/" and no parent), and register it with adapter.
    auto root = make_shared<MDirectory>("/", nullptr);
    root->activate(adapter);

    // Create a file called "README" in the root directory.
    auto file = make_shared<MFile>("README", root);
    file->writeDirect({{"This file system contains a collection of poetry."}});
    file->activate(adapter);

    // Create a directory called "Coleridge" in the root directory.
    auto coleridge = make_shared<MDirectory>("Coleridge", root);
    coleridge->activate(adapter);

    // Create a file called "Kubla_Khan" in the Coleridge directory
    file = make_shared<MFile>("Kubla_Khan", coleridge);
    file->writeDirect(
        {"In Xanadu did Kubla Khan",
         "A stately pleasure-dome decree:",
         "Where Alph, the sacred river, ran",
         "Through caverns measureless to man",
         "Down to a sunless sea."});
    file->activate(adapter);

    // Start dispatching requests after registering all servants.
    adapter->activate();
    cout << "Listening on port 4061..." << endl;

    // Wait until the user presses Ctrl+C.
    int signal = ctrlCHandler.wait();
    cout << "Caught signal " << signal << ", exiting..." << endl;

    return 0;
}
