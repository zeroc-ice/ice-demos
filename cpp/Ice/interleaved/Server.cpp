// **********************************************************************
//
// Copyright (c) 2003-2016 ZeroC, Inc. All rights reserved.
//
// **********************************************************************

#include <Ice/Ice.h>
#include <ThroughputI.h>

using namespace std;

class ThroughputServer : public Ice::Application
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
    ThroughputServer app;
    return app.main(argc, argv, "config.server");
}

int
ThroughputServer::run(int argc, char*[])
{
    if(argc > 1)
    {
        cerr << appName() << ": too many arguments" << endl;
        return EXIT_FAILURE;
    }

    Ice::ObjectAdapterPtr adapter = communicator()->createObjectAdapter("Throughput");
    Demo::ThroughputPtr servant = new ThroughputI;
    adapter->add(servant, Ice::stringToIdentity("throughput"));
    adapter->activate();
    communicator()->waitForShutdown();
    return EXIT_SUCCESS;
}
