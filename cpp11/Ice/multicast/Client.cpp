// **********************************************************************
//
// Copyright (c) 2003-present ZeroC, Inc. All rights reserved.
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

int run(const shared_ptr<Ice::Communicator>&, const string&);

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
        Ice::CommunicatorHolder ich(argc, argv, "config.client");
        auto communicator = ich.communicator();

        ctrlCHandler.setCallback(
            [communicator](int)
            {
                communicator->destroy();
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
            status = run(communicator, argv[0]);
        }
    }
    catch(const std::exception& ex)
    {
        cerr << ex.what() << endl;
        status = 1;
    }

    return status;
}

int
run(const shared_ptr<Ice::Communicator>& communicator, const string& appName)
{
    auto adapter = communicator->createObjectAdapter("DiscoverReply");
    auto replyI = make_shared<DiscoverReplyI>();
    auto reply = Ice::uncheckedCast<DiscoverReplyPrx>(adapter->addWithUUID(replyI));
    adapter->activate();

    auto discover = Ice::uncheckedCast<DiscoverPrx>(communicator->propertyToProxy("Discover.Proxy")->ice_datagram());
    discover->lookup(reply);
    auto base = replyI->waitReply(2);
    if(!base)
    {
        cerr << appName << ": no replies" << endl;
        return 1;
    }
    auto hello = Ice::checkedCast<HelloPrx>(base);
    if(!hello)
    {
        cerr << appName << ": invalid reply" << endl;
        return 1;
    }

    hello->sayHello();

    return 0;
}
