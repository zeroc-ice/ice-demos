// Copyright (c) ZeroC, Inc.

#include "SessionFactoryI.h"
#include <iostream>

using namespace std;
using namespace Demo;

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

        ctrlCHandler.setCallback([communicator](int) { communicator->shutdown(); });

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
            auto adapter = communicator->createObjectAdapter("SessionFactory");
            auto factory = make_shared<SessionFactoryI>();
            adapter->add(factory, Ice::stringToIdentity("SessionFactory"));
            adapter->activate();

            communicator->waitForShutdown();
        }
    }
    catch (const std::exception& ex)
    {
        cerr << ex.what() << endl;
        status = 1;
    }

    return status;
}
