// **********************************************************************
//
// Copyright (c) 2003-present ZeroC, Inc. All rights reserved.
//
// **********************************************************************

#include <Ice/Ice.h>
#include <FilesystemI.h>

using namespace std;
using namespace Filesystem;
using namespace FilesystemI;

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
            //
            // Create an object adapter.
            //
            auto adapter =
                communicator->createObjectAdapterWithEndpoints("SimpleFilesystem", "default -h 127.0.0.1 -p 10000");

            //
            // Create the root directory.
            //
            auto root = make_shared<DirectoryI>();
            Ice::Identity id;
            id.name = "RootDir";
            adapter->add(root, id);

            //
            // All objects are created, allow client requests now.
            //
            adapter->activate();

            //
            // Wait until we are done.
            //
            communicator->waitForShutdown();
        }
    }
    catch(const std::exception& ex)
    {
        cerr << ex.what() << endl;
        status = 1;
    }

    return status;
}
