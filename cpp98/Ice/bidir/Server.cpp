// **********************************************************************
//
// Copyright (c) 2003-2018 ZeroC, Inc. All rights reserved.
//
// **********************************************************************

#include <Ice/Ice.h>
#include <CallbackI.h>

using namespace std;
using namespace Demo;

class CallbackServer : public Ice::Application
{
public:

    virtual int run(int, char*[]);
};

int
main(int argc, char* argv[])
{
#ifdef ICE_STATIC_LIBS
    Ice::registerIceWS();
#endif

    CallbackServer app;
    return app.main(argc, argv, "config.server");
}

int
CallbackServer::run(int argc, char*[])
{
    if(argc > 1)
    {
        cerr << appName() << ": too many arguments" << endl;
        return EXIT_FAILURE;
    }

    Ice::ObjectAdapterPtr adapter = communicator()->createObjectAdapter("Callback.Server");
    CallbackSenderIPtr sender = new CallbackSenderI;
    adapter->add(sender, Ice::stringToIdentity("sender"));
    adapter->activate();

    sender->start();
    communicator()->waitForShutdown();
    sender->destroy();

    return EXIT_SUCCESS;
}
