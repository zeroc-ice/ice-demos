// **********************************************************************
//
// Copyright (c) 2003-2018 ZeroC, Inc. All rights reserved.
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

class HelloServer : public Ice::Application
{
public:

    virtual int run(int, char*[]) override;
};

int
main(int argc, char* argv[])
{
#ifdef ICE_STATIC_LIBS
    Ice::registerIceUDP();
#endif

    HelloServer app;
    return app.main(argc, argv, "config.server");
}

int
HelloServer::run(int, char*[])
{
    auto adapter = communicator()->createObjectAdapter("Hello");
    auto discoverAdapter = communicator()->createObjectAdapter("Discover");

    auto hello = adapter->addWithUUID(make_shared<HelloI>());
    auto d = make_shared<DiscoverI>(hello);
    discoverAdapter->add(d, Ice::stringToIdentity("discover"));

    discoverAdapter->activate();
    adapter->activate();

    communicator()->waitForShutdown();
    return EXIT_SUCCESS;
}
