// **********************************************************************
//
// Copyright (c) 2003-2017 ZeroC, Inc. All rights reserved.
//
// **********************************************************************

#include <Ice/Ice.h>
#include <ContextI.h>

using namespace std;

class ContextServer : public Ice::Application
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
    ContextServer app;
    return app.main(argc, argv, "config.server");
}

int
ContextServer::run(int argc, char*[])
{
    if(argc > 1)
    {
        cerr << appName() << ": too many arguments" << endl;
        return EXIT_FAILURE;
    }

    auto adapter = communicator()->createObjectAdapter("Context");
    adapter->add(make_shared<ContextI>(), Ice::stringToIdentity("context"));
    adapter->activate();
    communicator()->waitForShutdown();
    return EXIT_SUCCESS;
}
