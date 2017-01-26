// **********************************************************************
//
// Copyright (c) 2003-2016 ZeroC, Inc. All rights reserved.
//
// **********************************************************************

#include <Ice/Ice.h>
#include <NestedI.h>

using namespace std;
using namespace Demo;

class NestedServer : public Ice::Application
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
    NestedServer app;
    return app.main(argc, argv, "config.server");
}

int
NestedServer::run(int argc, char*[])
{
    if(argc > 1)
    {
        cerr << appName() << ": too many arguments" << endl;
        return EXIT_FAILURE;
    }

    Ice::ObjectAdapterPtr adapter = communicator()->createObjectAdapter("Nested.Server");
    NestedPrx self = NestedPrx::uncheckedCast(adapter->createProxy(Ice::stringToIdentity("nestedServer")));
    NestedPtr servant = new NestedI(self);
    adapter->add(servant, Ice::stringToIdentity("nestedServer"));
    adapter->activate();
    communicator()->waitForShutdown();
    return EXIT_SUCCESS;
}
