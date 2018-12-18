// **********************************************************************
//
// Copyright (c) 2003-present ZeroC, Inc. All rights reserved.
//
// **********************************************************************

#include <Ice/Ice.h>
#include <FilesystemI.h>

using namespace std;
using namespace Filesystem;

int run();

//
// Global variable for shutdownCommunicator
//
Ice::CommunicatorPtr communicator;

//
// Callback for CtrlCHandler
//
void
shutdownCommunicator(int)
{
    communicator->shutdown();
    cerr << "received signal, shutting down" << endl;
}

int
main(int argc, char* argv[])
{
    int status = 0;

    try
    {
        //
        // CtrlCHandler must be created before the communicator or any other threads are started
        //
        Ice::CtrlCHandler ctrlCHandler;

        //
        // CommunicatorHolder's ctor initializes an Ice communicator,
        // and it's dtor destroys this communicator.
        //
        Ice::CommunicatorHolder ich(argc, argv);
        communicator = ich.communicator();

        //
        // Shutdown communicator on Ctrl-C
        //
        ctrlCHandler.setCallback(&shutdownCommunicator);

        //
        // The communicator initialization removes all Ice-related arguments from argc/argv
        //
        if(argc > 1)
        {
            cerr << argv[0] << ": too many arguments" << endl;
            status = 1;
        }
        else
        {
            status = run();
        }
    }
    catch(const std::exception& ex)
    {
        cerr << ex.what() << endl;
        status = 1;
    }

    return status;
}

int
run()
{
    //
    // Create an object adapter.
    //
    Ice::ObjectAdapterPtr adapter =
        communicator->createObjectAdapterWithEndpoints("SimpleFilesystem", "default -h localhost -p 10000");

    //
    // Create the root directory (with name "/" and no parent)
    //
    DirectoryIPtr root = new DirectoryI("/", 0);
    root->activate(adapter);

    //
    // Create a file called "README" in the root directory
    //
    FileIPtr file = new FileI("README", root);
    Lines text;
    text.push_back("This file system contains a collection of poetry.");
    file->write(text);
    file->activate(adapter);

    //
    // Create a directory called "Coleridge" in the root directory
    //
    DirectoryIPtr coleridge = new DirectoryI("Coleridge", root);
    coleridge->activate(adapter);

    //
    // Create a file called "Kubla_Khan" in the Coleridge directory
    //
    file = new FileI("Kubla_Khan", coleridge);
    text.erase(text.begin(), text.end());
    text.push_back("In Xanadu did Kubla Khan");
    text.push_back("A stately pleasure-dome decree:");
    text.push_back("Where Alph, the sacred river, ran");
    text.push_back("Through caverns measureless to man");
    text.push_back("Down to a sunless sea.");
    file->write(text);
    file->activate(adapter);

    //
    // All objects are created, allow client requests now
    //
    adapter->activate();

    //
    // Wait until we are done
    //
    communicator->waitForShutdown();
    return 0;
}
