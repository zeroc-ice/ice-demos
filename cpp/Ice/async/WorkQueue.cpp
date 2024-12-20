//
// Copyright (c) ZeroC, Inc. All rights reserved.
//

#include "WorkQueue.h"
#include <Ice/Ice.h>
#include <iostream>

using namespace std;

void
WorkQueue::start()
{
    thread t([this]() { this->run(); });
    _thread = std::move(t);
}

void
WorkQueue::join()
{
    if (_thread.joinable())
    {
        _thread.join();
    }
}

void
WorkQueue::run()
{
    unique_lock<mutex> lock(_mutex);

    while (!_done)
    {
        if (_callbacks.empty())
        {
            _condition.wait(lock);
        }

        if (!_done && !_callbacks.empty())
        {
            //
            // Get next work item.
            //
            CallbackEntry entry = _callbacks.front();

            //
            // Wait for the amount of time indicated in delay to
            // emulate a process that takes a significant period of
            // time to complete.
            //
            _condition.wait_for(lock, chrono::milliseconds(get<0>(entry)));

            if (!_done)
            {
                //
                // Print greeting and send response.
                //
                _callbacks.pop_front();
                cout << "Belated Hello World!" << endl;
                auto& response = get<1>(entry);
                response();
            }
        }
    }

    //
    // Throw exception for any outstanding requests.
    //
    for (auto& entry : _callbacks)
    {
        get<2>(entry)(make_exception_ptr(Demo::RequestCanceledException()));
    }
}

void
WorkQueue::add(int delay, function<void()> response, function<void(exception_ptr)> error)
{
    const unique_lock<mutex> lock(_mutex);

    if (!_done)
    {
        //
        // Add work item.
        //
        if (_callbacks.size() == 0)
        {
            _condition.notify_one();
        }
        _callbacks.emplace_back(delay, std::move(response), std::move(error));
    }
    else
    {
        //
        // Destroyed, throw exception.
        //
        error(make_exception_ptr(Demo::RequestCanceledException()));
    }
}

void
WorkQueue::destroy()
{
    const unique_lock<mutex> lock(_mutex);

    //
    // Set done flag and notify.
    //
    _done = true;
    _condition.notify_one();
}
