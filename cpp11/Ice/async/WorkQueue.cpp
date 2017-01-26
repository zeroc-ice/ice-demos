// **********************************************************************
//
// Copyright (c) 2003-2016 ZeroC, Inc. All rights reserved.
//
// **********************************************************************

#include <Ice/Ice.h>
#include <WorkQueue.h>

using namespace std;

WorkQueue::WorkQueue() : _done(false)
{
}

void
WorkQueue::start()
{
    thread t([this]()
        {
            this->run();
        });
    _thread = move(t);
}

void
WorkQueue::join()
{
    if(_thread.joinable())
    {
        _thread.join();
    }
}

void
WorkQueue::run()
{
    unique_lock<mutex> lock(_mutex);

    while(!_done)
    {
        if(_callbacks.empty())
        {
            _condition.wait(lock);
        }

        if(!_callbacks.empty())
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

            if(!_done)
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
    for(auto& entry : _callbacks)
    {
        try
        {
            throw Demo::RequestCanceledException();
        }
        catch(...)
        {
            auto& error = get<2>(entry);
            error(current_exception());
        }
    }
}

void
WorkQueue::add(int delay, function<void ()> response, function<void (exception_ptr)> error)
{
    unique_lock<mutex> lock(_mutex);

    if(!_done)
    {
        //
        // Add work item.
        //
        if(_callbacks.size() == 0)
        {
            _condition.notify_one();
        }
        _callbacks.push_back(make_tuple(delay, move(response), move(error)));
    }
    else
    {
        //
        // Destroyed, throw exception.
        //
        try
        {
            throw Demo::RequestCanceledException();
        }
        catch(...)
        {
            error(current_exception());
        }
    }
}

void
WorkQueue::destroy()
{
    unique_lock<mutex> lock(_mutex);

    //
    // Set done flag and notify.
    //
    _done = true;
    _condition.notify_one();
}
