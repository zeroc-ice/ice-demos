// **********************************************************************
//
// Copyright (c) 2003-present ZeroC, Inc. All rights reserved.
//
// **********************************************************************

#include <Ice/Ice.h>
#include <HelloI.h>
#include <WorkQueue.h>

using namespace std;

//
// Global variables for shutdownCommunicator
//
Ice::CommunicatorPtr communicator;
WorkQueuePtr _workQueue;

//
// Callback for CtrlCHandler
//
void
shutdownCommunicator(int)
{
    _workQueue->destroy();
    communicator->shutdown();
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
        Ice::CommunicatorHolder ich(argc, argv, "config.server");
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
            Ice::ObjectAdapterPtr adapter = communicator->createObjectAdapter("Hello");
            _workQueue = new WorkQueue();
            Demo::HelloPtr hello = new HelloI(_workQueue);
            adapter->add(hello, Ice::stringToIdentity("hello"));

            _workQueue->start();
            adapter->activate();

            communicator->waitForShutdown();
            _workQueue->getThreadControl().join();
        }
    }
    catch(const std::exception& ex)
    {
        cerr << ex.what() << endl;
        status = 1;
    }

    return status;
}
