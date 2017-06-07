// **********************************************************************
//
// Copyright (c) 2003-2017 ZeroC, Inc. All rights reserved.
//
// **********************************************************************

#include <Ice/Ice.h>
#include <NestedI.h>

using namespace std;
using namespace Demo;

class NestedServer : public Ice::Application
{
public:

    virtual int run(int, char*[]) override;
};

int
#ifdef _WIN32
wmain(int argc, wchar_t* argv[])
#else
main(int argc, char* argv[])
#endif
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

    auto adapter = communicator()->createObjectAdapter("Nested.Server");
    auto self = Ice::uncheckedCast<NestedPrx>(adapter->createProxy(Ice::stringToIdentity("nestedServer")));
    auto servant = make_shared<NestedI>(self);
    adapter->add(servant, Ice::stringToIdentity("nestedServer"));
    adapter->activate();
    communicator()->waitForShutdown();
    return EXIT_SUCCESS;
}
