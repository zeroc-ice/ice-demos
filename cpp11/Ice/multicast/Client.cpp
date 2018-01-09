// **********************************************************************
//
// Copyright (c) 2003-2018 ZeroC, Inc. All rights reserved.
//
// **********************************************************************

#include <Ice/Ice.h>

#include <Hello.h>
#include <Discovery.h>

#include <mutex>
#include <condition_variable>
#include <chrono>

using namespace std;
using namespace Demo;

class DiscoverReplyI : public DiscoverReply
{
public:

    virtual void
    reply(shared_ptr<Ice::ObjectPrx> obj, const Ice::Current&) override
    {
        {
            lock_guard<mutex> lk(_mutex);
            if(!_obj)
            {
                _obj = obj;
            }
        }
        _cond.notify_all();
    }

    shared_ptr<Ice::ObjectPrx>
    waitReply(int seconds)
    {
        auto until = chrono::system_clock::now() + chrono::seconds(seconds);
        unique_lock<mutex> lk(_mutex);
        bool timedOut = false;
        do
        {
            timedOut = _cond.wait_until(lk, until) == cv_status::timeout;
        } while(!_obj && !timedOut);

        return _obj;
    }

private:

    shared_ptr<Ice::ObjectPrx> _obj;
    mutex _mutex;
    condition_variable _cond;
};

class HelloClient : public Ice::Application
{
public:

    virtual int run(int, char*[]) override;

private:

    void menu();
};

int
main(int argc, char* argv[])
{
#ifdef ICE_STATIC_LIBS
    Ice::registerIceUDP();
#endif

    HelloClient app;
    return app.main(argc, argv, "config.client");
}

int
HelloClient::run(int, char* argv[])
{
    auto adapter = communicator()->createObjectAdapter("DiscoverReply");
    auto replyI = make_shared<DiscoverReplyI>();
    auto reply = Ice::uncheckedCast<DiscoverReplyPrx>(adapter->addWithUUID(replyI));
    adapter->activate();

    auto discover = Ice::uncheckedCast<DiscoverPrx>(communicator()->propertyToProxy("Discover.Proxy")->ice_datagram());
    discover->lookup(reply);
    auto base = replyI->waitReply(2);
    if(!base)
    {
        cerr << argv[0] << ": no replies" << endl;
        return 1;
    }
    auto hello = Ice::checkedCast<HelloPrx>(base);
    if(!hello)
    {
        cerr << argv[0] << ": invalid reply" << endl;
        return 1;
    }

    hello->sayHello();

    return 0;
}
