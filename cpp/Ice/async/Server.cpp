//
// Copyright (c) ZeroC, Inc. All rights reserved.
//

#include "HelloI.h"
#include "WorkQueue.h"
#include <Ice/Ice.h>
#include <iostream>

using namespace std;

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
        // and its dtor destroys this communicator.
        //
        const Ice::CommunicatorHolder ich(argc, argv, "config.server");
        const auto& communicator = ich.communicator();

        shared_ptr<WorkQueue> _workQueue;

        ctrlCHandler.setCallback(
            [communicator, _workQueue](int)
            {
                _workQueue->destroy();
                communicator->shutdown();
            });

        //
        // The communicator initialization removes all Ice-related arguments from argc/argv
        //
        if (argc > 1)
        {
            cerr << argv[0] << ": too many arguments" << endl;
            status = 1;
        }
        else
        {
            auto adapter = communicator->createObjectAdapter("Hello");
            _workQueue = make_shared<WorkQueue>();
            adapter->add(make_shared<HelloI>(_workQueue), Ice::stringToIdentity("hello"));

            _workQueue->start();
            adapter->activate();

            communicator->waitForShutdown();
            _workQueue->join();
        }
    }
    catch (const std::exception& ex)
    {
        cerr << ex.what() << endl;
        status = 1;
    }

    return status;
}
