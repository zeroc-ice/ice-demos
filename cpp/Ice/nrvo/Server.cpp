// **********************************************************************
//
// Copyright (c) 2003-2016 ZeroC, Inc. All rights reserved.
//
// **********************************************************************

#include <Ice/Ice.h>
#include <NrvoI.h>

using namespace std;

class NrvotServer : public Ice::Application
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
    NrvotServer app;
    return app.main(argc, argv, "config.server");
}

int
NrvotServer::run(int argc, char*[])
{
    if(argc > 1)
    {
        cerr << appName() << ": too many arguments" << endl;
        return EXIT_FAILURE;
    }

    Ice::ObjectAdapterPtr adapter = communicator()->createObjectAdapter("Nrvo");
    adapter->add(new NrvoI(), Ice::stringToIdentity("nrvo"));
    adapter->activate();
    communicator()->waitForShutdown();
    return EXIT_SUCCESS;
}
