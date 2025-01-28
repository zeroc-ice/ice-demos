// Copyright (c) ZeroC, Inc.

#include "Discovery.h"
#include "Hello.h"
#include <Ice/Ice.h>
#include <chrono>
#include <condition_variable>
#include <iostream>
#include <mutex>

using namespace std;
using namespace Demo;

class DiscoverReplyI final : public DiscoverReply
{
public:
    void reply(optional<Ice::ObjectPrx> obj, const Ice::Current&) final
    {
        {
            const lock_guard<mutex> lock(_mutex);
            if (!_obj)
            {
                _obj = obj;
            }
        }
        _cond.notify_all();
    }

    optional<Ice::ObjectPrx> waitReply(int seconds)
    {
        auto until = chrono::system_clock::now() + chrono::seconds(seconds);
        unique_lock<mutex> lock(_mutex);
        bool timedOut = false;
        do
        {
            timedOut = _cond.wait_until(lock, until) == cv_status::timeout;
        } while (!_obj && !timedOut);

        return _obj;
    }

private:
    optional<Ice::ObjectPrx> _obj;
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
    auto adapter = communicator->createObjectAdapter("DiscoverReply");
    auto replyI = make_shared<DiscoverReplyI>();
    auto reply = Ice::uncheckedCast<DiscoverReplyPrx>(adapter->addWithUUID(replyI));
    adapter->activate();

    auto discover = Ice::uncheckedCast<DiscoverPrx>(communicator->propertyToProxy("Discover.Proxy")->ice_datagram());
    discover->lookup(reply);
    auto base = replyI->waitReply(2);
    if (!base)
    {
        cerr << appName << ": no replies" << endl;
        return 1;
    }
    auto hello = Ice::checkedCast<HelloPrx>(base);
    if (!hello)
    {
        cerr << appName << ": invalid reply" << endl;
        return 1;
    }

    hello->sayHello();

    return 0;
}
