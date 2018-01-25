// **********************************************************************
//
// Copyright (c) 2003-2018 ZeroC, Inc. All rights reserved.
//
// **********************************************************************

#include <Ice/Ice.h>
#include <ThroughputI.h>

using namespace std;

class InterleavedServer : public Ice::Application
{
public:

    virtual int run(int, char*[]) override;
};

int
main(int argc, char* argv[])
{
#ifdef ICE_STATIC_LIBS
    Ice::registerIceSSL();
#endif
    InterleavedServer app;
    return app.main(argc, argv, "config.server");
}

int
InterleavedServer::run(int argc, char*[])
{
    if(argc > 1)
    {
        cerr << appName() << ": too many arguments" << endl;
        return EXIT_FAILURE;
    }

    auto adapter = communicator()->createObjectAdapter("Throughput");
    auto servant = make_shared<ThroughputI>();
    adapter->add(servant, Ice::stringToIdentity("throughput"));
    adapter->activate();
    communicator()->waitForShutdown();
    return EXIT_SUCCESS;
}
