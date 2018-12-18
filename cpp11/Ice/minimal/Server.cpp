// **********************************************************************
//
// Copyright (c) 2003-present ZeroC, Inc. All rights reserved.
//
// **********************************************************************

#include <Ice/Ice.h>
#include <HelloI.h>

using namespace std;

int
main(int argc, char* argv[])
{
    try
    {
        //
        // CtrlCHandler must be created before the communicator or any other threads are started
        //
        Ice::CtrlCHandler ctrlCHandler;

        Ice::CommunicatorHolder ich(argc, argv);
        auto communicator = ich.communicator();

        ctrlCHandler.setCallback(
            [communicator](int)
            {
                communicator->shutdown();
            });

        auto adapter = communicator->createObjectAdapterWithEndpoints("Hello", "default -h localhost -p 10000");
        adapter->add(make_shared<HelloI>(), Ice::stringToIdentity("hello"));
        adapter->activate();
        communicator->waitForShutdown();
    }
    catch(const std::exception& ex)
    {
        cerr << ex.what() << endl;
        return 1;
    }
    return 0;
}
