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
    sayHello(const Ice::Current&)
    {
        cout << "Hello World!" << endl;
    }
};

class DiscoverI : public Discover
{
public:

    DiscoverI(const Ice::ObjectPrx& obj) :
        _obj(obj)
    {
    }

    virtual void
    lookup(const DiscoverReplyPrx& reply, const Ice::Current&)
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

    // Required to prevent compiler warnings with MSVC++
    DiscoverI& operator=(const DiscoverI&);

    const Ice::ObjectPrx _obj;
};

//
// Global variable for shutdownCommunicator
//
Ice::CommunicatorPtr communicator;

//
// Callback for CtrlCHandler
//
void
shutdownCommunicator(int)
{
    communicator->shutdown();
}

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
        // and it's dtor destroys this communicator.
        //
        Ice::CommunicatorHolder ich(argc, argv, "config.server");
        communicator = ich.communicator();

        //
        // Shutdown communicator on Ctrl-C
        //
        ctrlCHandler.setCallback(&shutdownCommunicator);

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
            Ice::ObjectAdapterPtr adapter = communicator->createObjectAdapter("Hello");
            Ice::ObjectAdapterPtr discoverAdapter = communicator->createObjectAdapter("Discover");

            Ice::ObjectPrx hello = adapter->addWithUUID(new HelloI);
            DiscoverPtr d = new DiscoverI(hello);
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
