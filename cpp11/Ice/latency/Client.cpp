// **********************************************************************
//
// Copyright (c) 2003-present ZeroC, Inc. All rights reserved.
//
// **********************************************************************

#include <Ice/Ice.h>
#include <Latency.h>

using namespace std;
using namespace Demo;

int run(const shared_ptr<Ice::Communicator>&, const string&);

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
        // and its dtor destroys this communicator.
        //
        Ice::CommunicatorHolder ich(argc, argv, "config.client");
        auto communicator = ich.communicator();

        ctrlCHandler.setCallback(
            [communicator](int)
            {
                communicator->destroy();
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
            status = run(communicator, argv[0]);
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
run(const shared_ptr<Ice::Communicator>& communicator, const string& appName)
{
    auto ping =  Ice::checkedCast<PingPrx>(communicator->propertyToProxy("Ping.Proxy"));
    if(!ping)
    {
        cerr << appName << ": invalid proxy" << endl;
        return 1;
    }

    // Initial ping to setup the connection.
    ping->ice_ping();

    auto start = chrono::high_resolution_clock::now();

    const int repetitions = 100000;
    cout << "pinging server " << repetitions << " times (this may take a while)" << endl;
    for(int i = 0; i < repetitions; ++i)
    {
        ping->ice_ping();
    }

    auto duration = chrono::duration<float, chrono::milliseconds::period>(chrono::high_resolution_clock::now() - start);
    cout << "time for " << repetitions << " pings: " << duration.count() << "ms" << endl;
    cout << "time per ping: " << duration.count() / repetitions << "ms" << endl;

    return 0;
}
