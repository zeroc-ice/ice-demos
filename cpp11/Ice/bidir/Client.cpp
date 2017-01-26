// **********************************************************************
//
// Copyright (c) 2003-2016 ZeroC, Inc. All rights reserved.
//
// **********************************************************************

#include <Ice/Ice.h>
#include <Callback.h>

using namespace std;
using namespace Demo;

class CallbackReceiverI : public CallbackReceiver
{
public:

    virtual void
    callback(Ice::Int num, const Ice::Current&)
    {
        cout << "received callback #" << num << endl;
    }
};

class CallbackClient : public Ice::Application
{
public:

    virtual int run(int, char*[]);
};

int
main(int argc, char* argv[])
{
    CallbackClient app;
    return app.main(argc, argv, "config.client");
}

int
CallbackClient::run(int argc, char*[])
{
    if(argc > 1)
    {
        cerr << appName() << ": too many arguments" << endl;
        return EXIT_FAILURE;
    }

    auto server = Ice::checkedCast<CallbackSenderPrx>(communicator()->propertyToProxy("CallbackSender.Proxy"));
    if(!server)
    {
        cerr << appName() << ": invalid proxy" << endl;
        return EXIT_FAILURE;
    }

    auto adapter = communicator()->createObjectAdapter("");
    Ice::Identity ident{ Ice::generateUUID(), ""};
    adapter->add(make_shared<CallbackReceiverI>(), ident);
    adapter->activate();
    server->ice_getConnection()->setAdapter(adapter);
    server->addClient(ident);
    communicator()->waitForShutdown();

    return EXIT_SUCCESS;
}
