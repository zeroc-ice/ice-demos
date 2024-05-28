//
// Copyright (c) ZeroC, Inc. All rights reserved.
//

#include "Discovery.h"
#include "Hello.h"
#include <Ice/Ice.h>
#include <iostream>

using namespace std;
using namespace Demo;

class HelloI final : public Hello
{
public:
    void sayHello(const Ice::Current&) final { cout << "Hello World!" << endl; }
};

class DiscoverI final : public Discover
{
public:
    DiscoverI(const optional<Ice::ObjectPrx>& obj) : _obj(obj) {}

    void lookup(optional<DiscoverReplyPrx> reply, const Ice::Current&) final
    {
        try
        {
            if (reply)
            {
                reply->reply(_obj);
            }
        }
        catch (const Ice::LocalException&)
        {
            // Ignore
        }
    }

private:
    const optional<Ice::ObjectPrx> _obj;
};

int
main(int argc, char* argv[])
{
#ifdef ICE_STATIC_LIBS
    Ice::registerIceUDP();
#endif

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
        const Ice::CommunicatorHolder ich(argc, argv, "config.server");
        const auto& communicator = ich.communicator();

        ctrlCHandler.setCallback([communicator](int) { communicator->shutdown(); });

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
            auto adapter = communicator->createObjectAdapter("Hello");
            auto discoverAdapter = communicator->createObjectAdapter("Discover");

            auto hello = adapter->addWithUUID(make_shared<HelloI>());
            auto d = make_shared<DiscoverI>(hello);
            discoverAdapter->add(d, Ice::stringToIdentity("discover"));

            discoverAdapter->activate();
            adapter->activate();

            communicator->waitForShutdown();
        }
    }
    catch (const std::exception& ex)
    {
        cerr << ex.what() << endl;
        status = 1;
    }

    return status;
}
