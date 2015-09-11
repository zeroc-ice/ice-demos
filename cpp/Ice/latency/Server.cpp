// **********************************************************************
//
// Copyright (c) 2003-2015 ZeroC, Inc. All rights reserved.
//
// **********************************************************************

#include <Ice/Ice.h>
#include <Latency.h>

using namespace std;
using namespace Demo;

class LatencyServer : public Ice::Application
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
    LatencyServer app;
    return app.main(argc, argv, "config.server");
}

int
LatencyServer::run(int argc, char*[])
{
    if(argc > 1)
    {
        cerr << appName() << ": too many arguments" << endl;
        return EXIT_FAILURE;
    }

    Ice::ObjectAdapterPtr adapter = communicator()->createObjectAdapter("Latency");
    Ice::ObjectPtr object = new Ping;
    adapter->add(object, communicator()->stringToIdentity("ping"));
    adapter->activate();
    communicator()->waitForShutdown();
    return EXIT_SUCCESS;
}
