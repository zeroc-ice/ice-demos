// **********************************************************************
//
// Copyright (c) 2003-2016 ZeroC, Inc. All rights reserved.
//
// **********************************************************************

#include <Ice/Ice.h>
#include <Latency.h>

using namespace std;
using namespace Demo;

class LatencyClient : public Ice::Application
{
public:

    virtual int run(int, char*[]);
};


int
main(int argc, char* argv[])
{
#ifdef ICE_STATIC_LIBS
    Ice::registerIceSSL();
#endif
    LatencyClient app;
    return app.main(argc, argv, "config.client");
}

int
LatencyClient::run(int argc, char* argv[])
{
    if(argc > 1)
    {
        cerr << appName() << ": too many arguments" << endl;
        return EXIT_FAILURE;
    }

    auto ping =  Ice::checkedCast<PingPrx>(communicator()->propertyToProxy("Ping.Proxy"));
    if(!ping)
    {
        cerr << argv[0] << ": invalid proxy" << endl;
        return EXIT_FAILURE;
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

    return EXIT_SUCCESS;
}
