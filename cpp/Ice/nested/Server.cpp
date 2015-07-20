// **********************************************************************
//
// Copyright (c) 2003-2015 ZeroC, Inc. All rights reserved.
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

#ifdef ICE_STATIC_LIBS
extern "C"
{

Ice::Plugin* createIceSSL(const Ice::CommunicatorPtr&, const string&, const Ice::StringSeq&);

}
#endif

int
main(int argc, char* argv[])
{
#ifdef ICE_STATIC_LIBS
    Ice::registerPluginFactory("IceSSL", createIceSSL, false);
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
    NestedPrx self = NestedPrx::uncheckedCast(adapter->createProxy(communicator()->stringToIdentity("nestedServer")));
    NestedPtr servant = new NestedI(self);
    adapter->add(servant, communicator()->stringToIdentity("nestedServer"));
    adapter->activate();
    communicator()->waitForShutdown();
    return EXIT_SUCCESS;
}
