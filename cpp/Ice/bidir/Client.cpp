//
// Copyright (c) ZeroC, Inc. All rights reserved.
//

#include "Callback.h"
#include <Ice/Ice.h>
#include <iostream>

using namespace std;
using namespace Demo;

class CallbackReceiverI final : public CallbackReceiver
{
public:
    void callback(int32_t num, const Ice::Current&) final { cout << "received callback #" << num << endl; }
};

int run(const shared_ptr<Ice::Communicator>&, const string&);

int
main(int argc, char* argv[])
{
    int status = 0;

    try
    {
        //
        // CtrlCHandler must be created before the communicator or any other threads are started
        //
        Ice::CtrlCHandler ctrlCHandler;

        //
        // CommunicatorHolder's ctor initializes an Ice communicator,
        // and its dtor destroys this communicator.
        //
        const Ice::CommunicatorHolder ich(argc, argv, "config.client");
        const auto& communicator = ich.communicator();

        ctrlCHandler.setCallback([communicator](int) { communicator->destroy(); });

        //
        // The communicator initialization removes all Ice-related arguments from argc/argv
        //
        if (argc > 1)
        {
            cerr << argv[0] << ": too many arguments" << endl;
            status = 1;
        }
        else
        {
            status = run(communicator, argv[0]);
        }
    }
    catch (const std::exception& ex)
    {
        cerr << ex.what() << endl;
        status = 1;
    }

    return status;
}

int
run(const shared_ptr<Ice::Communicator>& communicator, const string& appName)
{
    auto server = Ice::checkedCast<CallbackSenderPrx>(communicator->propertyToProxy("CallbackSender.Proxy"));
    if (!server)
    {
        cerr << appName << ": invalid proxy" << endl;
        return 1;
    }

    //
    // Create an object adapter with no name and no endpoints for receiving callbacks
    // over bidirectional connections.
    //
    auto adapter = communicator->createObjectAdapter("");

    //
    // Register the callback receiver servant with the object adapter and activate
    // the adapter.
    //
    auto proxy = Ice::uncheckedCast<CallbackReceiverPrx>(adapter->addWithUUID(make_shared<CallbackReceiverI>()));
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
    communicator->waitForShutdown();

    return 0;
}
