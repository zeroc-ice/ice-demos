// **********************************************************************
//
// Copyright (c) 2003-present ZeroC, Inc. All rights reserved.
//
// **********************************************************************

#include <Ice/Ice.h>

#include <Discovery.h>
#include <Hello.h>

using namespace std;
using namespace Demo;

class HelloI : public Hello
{
public:

    virtual void
    sayHello(const Ice::Current&) override
    {
        cout << "Hello World!" << endl;
    }
};

class DiscoverI : public Discover
{
public:

    DiscoverI(const shared_ptr<Ice::ObjectPrx>& obj) :
        _obj(obj)
    {
    }

    virtual void
    lookup(shared_ptr<DiscoverReplyPrx> reply, const Ice::Current&) override
    {
        try
        {
            reply->reply(_obj);
        }
        catch(const Ice::LocalException&)
        {
            // Ignore
        }
    }

private:

    const shared_ptr<Ice::ObjectPrx> _obj;
};

int main(int argc, char* argv[])
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
        Ice::CommunicatorHolder ich(argc, argv, "config.server");
        auto communicator = ich.communicator();

        ctrlCHandler.setCallback(
            [communicator](int)
            {
                communicator->shutdown();
            });

        //
        // The communicator initialization removes all Ice-related arguments from argc/argv
        //
        if(argc > 1)
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
    catch(const std::exception& ex)
    {
        cerr << ex.what() << endl;
        status = 1;
    }

    return status;
}
