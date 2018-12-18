// **********************************************************************
//
// Copyright (c) 2003-present ZeroC, Inc. All rights reserved.
//
// **********************************************************************

#include <Ice/Ice.h>
#include <FilesystemI.h>

using namespace std;
using namespace Filesystem;

int run(const shared_ptr<Ice::Communicator>& communicator);

int main(int argc, char* argv[])
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
        // and its dtor destroys this communicator.
        //
        Ice::CommunicatorHolder ich(argc, argv);
        auto communicator = ich.communicator();

        auto appName = argv[0];
        ctrlCHandler.setCallback(
            [communicator, appName](int)
            {
                communicator->shutdown();
                cerr << appName << ": received signal, shutting down" << endl;
            });

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
            status = run(communicator);
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
run(const shared_ptr<Ice::Communicator>& communicator)
{
    //
    // Create an object adapter.
    //
    auto adapter = communicator->createObjectAdapterWithEndpoints(
        "SimpleFilesystem", "default -h localhost -p 10000");

    //
    // Create the root directory (with name "/" and no parent)
    //
    auto root = make_shared<DirectoryI>("/", nullptr);
    root->activate(adapter);

    //
    // Create a file called "README" in the root directory
    //
    auto file = make_shared<FileI>("README", root);
    auto text = Lines({"This file system contains a collection of poetry."});
    file->write(text, Ice::emptyCurrent);
    file->activate(adapter);

    //
    // Create a directory called "Coleridge" in the root directory
    //
    auto coleridge = make_shared<DirectoryI>("Coleridge", root);
    coleridge->activate(adapter);

    //
    // Create a file called "Kubla_Khan" in the Coleridge directory
    //
    file = make_shared<FileI>("Kubla_Khan", coleridge);
    text =
        {
            "In Xanadu did Kubla Khan",
            "A stately pleasure-dome decree:",
            "Where Alph, the sacred river, ran",
            "Through caverns measureless to man",
            "Down to a sunless sea."
        };
    file->write(text, Ice::emptyCurrent);
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
