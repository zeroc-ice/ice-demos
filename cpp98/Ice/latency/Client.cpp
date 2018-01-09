// **********************************************************************
//
// Copyright (c) 2003-2018 ZeroC, Inc. All rights reserved.
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
    Ice::registerIceWS();
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
        return 1;
    }

    PingPrx ping = PingPrx::checkedCast(communicator()->propertyToProxy("Ping.Proxy"));
    if(!ping)
    {
        cerr << argv[0] << ": invalid proxy" << endl;
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
