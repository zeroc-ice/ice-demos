// **********************************************************************
//
// Copyright (c) 2003-2018 ZeroC, Inc. All rights reserved.
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
#ifdef ICE_STATIC_LIBS
    Ice::registerIceWS();
#endif

    CallbackClient app;
    return app.main(argc, argv, "config.client");
}

int
CallbackClient::run(int argc, char*[])
{
    if(argc > 1)
    {
        cerr << appName() << ": too many arguments" << endl;
        return 1;
    }

    CallbackSenderPrx server = CallbackSenderPrx::checkedCast(communicator()->propertyToProxy("CallbackSender.Proxy"));
    if(!server)
    {
        cerr << appName() << ": invalid proxy" << endl;
        return 1;
    }

    //
    // Create an object adapter with no name and no endpoints for receiving callbacks
    // over bidirectional connections.
    //
    Ice::ObjectAdapterPtr adapter = communicator()->createObjectAdapter("");

    //
    // Register the callback receiver servant with the object adapter and activate
    // the adapter.
    //
    CallbackReceiverPrx proxy = CallbackReceiverPrx::uncheckedCast(adapter->addWithUUID(new CallbackReceiverI));
    adapter->activate();

    //
    // Associate the object adapter with the bidirectional connection.
    //
    server->ice_getConnection()->setAdapter(adapter);

    //
    // Provide the proxy of the callback receiver object to the server and wait for
    // shutdown.
    //
    server->addClient(proxy);
    communicator()->waitForShutdown();
    return 0;
}
