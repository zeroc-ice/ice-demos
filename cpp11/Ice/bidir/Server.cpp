// **********************************************************************
//
// Copyright (c) 2003-2016 ZeroC, Inc. All rights reserved.
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

    auto adapter = communicator()->createObjectAdapter("Callback.Server");
    auto sender = make_shared<CallbackSenderI>(communicator());
    adapter->add(sender, Ice::stringToIdentity("sender"));
    adapter->activate();

    sender->start();
    try
    {
        communicator()->waitForShutdown();
    }
    catch(...)
    {
        sender->destroy();
        throw;
    }
    sender->destroy();

    return EXIT_SUCCESS;
}
