//
// Copyright (c) ZeroC, Inc. All rights reserved.
//

#include "CallbackI.h"
#include <Ice/Ice.h>
#include <algorithm>
#include <iostream>

using namespace std;
using namespace Demo;

void
CallbackSenderI::addClient(optional<CallbackReceiverPrx> client, const Ice::Current& current)
{
    const lock_guard<mutex> lock(_mutex);
    cout << "adding client `" << Ice::identityToString(client->ice_getIdentity()) << "'" << endl;
    _clients.emplace_back(client->ice_fixed(current.con));
}

void
CallbackSenderI::destroy()
{
    {
        const lock_guard<mutex> lock(_mutex);
        cout << "destroying callback sender" << endl;
        _destroy = true;
    }
    _cv.notify_one();

    if (_result.valid())
    {
        try
        {
            // wait for async task to complete
            _result.get();
        }
        catch (const Ice::CommunicatorDestroyedException&)
        {
            // expected, if you CTRL-C the server while it's sending callbacks
        }
        catch (const std::exception& ex)
        {
            cerr << "sender task failed with: " << ex.what() << endl;
        }
    }
}

void
CallbackSenderI::start()
{
    assert(!_result.valid()); // start should only be called once
    _result = async(launch::async, [this] { invokeCallback(); });
}

void
CallbackSenderI::invokeCallback()
{
    int num = 0;
    unique_lock<mutex> lock(_mutex);
    while (!_destroy)
    {
        _cv.wait_for(lock, chrono::seconds(2));

        if (!_destroy && !_clients.empty())
        {
            ++num;

            //
            // Invoke callback on all clients; it's safe to do it with _mutex locked
            // because Ice guarantees these async invocations never block the calling thread.
            //
            // The exception callback, if called, is called by an Ice thread, and never the
            // calling thread.
            for (const auto& p : _clients)
            {
                auto self = shared_from_this();
                p->callbackAsync(num, nullptr, [self, p](const exception_ptr& eptr) { self->removeClient(p, eptr); });
            }
        }
    }
}

void
CallbackSenderI::removeClient(const optional<CallbackReceiverPrx>& client, const exception_ptr& eptr)
{
    const lock_guard<mutex> lock(_mutex);
    auto p = find(_clients.begin(), _clients.end(), client);
    if (p != _clients.end())
    {
        try
        {
            rethrow_exception(eptr);
        }
        catch (const Ice::Exception& ex)
        {
            cerr << "removing client `" << Ice::identityToString(client->ice_getIdentity()) << "':\n" << ex << endl;
        }
        _clients.erase(p);
    }
}
