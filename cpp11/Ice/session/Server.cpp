// **********************************************************************
//
// Copyright (c) 2003-2018 ZeroC, Inc. All rights reserved.
//
// **********************************************************************

#include <SessionFactoryI.h>

using namespace std;
using namespace Demo;

class SessionServer : public Ice::Application
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
    SessionServer app;
    return app.main(argc, argv, "config.server");
}

int
SessionServer::run(int argc, char*[])
{
    if(argc > 1)
    {
        cerr << appName() << ": too many arguments" << endl;
        return EXIT_FAILURE;
    }

    auto adapter = communicator()->createObjectAdapter("SessionFactory");
    auto factory = make_shared<SessionFactoryI>();
    adapter->add(factory, Ice::stringToIdentity("SessionFactory"));
    adapter->activate();
    communicator()->waitForShutdown();

    return EXIT_SUCCESS;
}
