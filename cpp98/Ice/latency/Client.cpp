// **********************************************************************
//
// Copyright (c) 2003-present ZeroC, Inc. All rights reserved.
//
// **********************************************************************

#include <Ice/Ice.h>
#include <Latency.h>

using namespace std;
using namespace Demo;

int run();

//
// Global variable for destroyCommunicator
//
Ice::CommunicatorPtr communicator;

//
// Callback for CtrlCHandler
//
void
destroyCommunicator(int)
{
    communicator->destroy();
}

int
main(int argc, char* argv[])
{
#ifdef ICE_STATIC_LIBS
    Ice::registerIceSSL();
    Ice::registerIceWS();
#endif
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
        Ice::CommunicatorHolder ich(argc, argv, "config.client");
        communicator = ich.communicator();

        //
        // Destroy communicator on Ctrl-C
        //
        ctrlCHandler.setCallback(&destroyCommunicator);

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
    PingPrx ping = PingPrx::checkedCast(communicator->propertyToProxy("Ping.Proxy"));
    if(!ping)
    {
        cerr << "invalid proxy" << endl;
        return 1;
    }

    // Initial ping to setup the connection.
    ping->ice_ping();

    IceUtil::Time tm = IceUtil::Time::now(IceUtil::Time::Monotonic);

    const int repetitions = 100000;
    cout << "pinging server " << repetitions << " times (this may take a while)" << endl;
    for(int i = 0; i < repetitions; ++i)
    {
        ping->ice_ping();
    }

    tm = IceUtil::Time::now(IceUtil::Time::Monotonic) - tm;

    cout << "time for " << repetitions << " pings: " << tm * 1000 << "ms" << endl;
    cout << "time per ping: " << tm * 1000 / repetitions << "ms" << endl;

    return 0;
}
