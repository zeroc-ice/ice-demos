// **********************************************************************
//
// Copyright (c) 2003-2016 ZeroC, Inc. All rights reserved.
//
// **********************************************************************

#include <Ice/Ice.h>
#include <HelloI.h>

using namespace std;

HelloI::HelloI(const shared_ptr<WorkQueue>& workQueue) :
    _workQueue(workQueue)
{
}

void
HelloI::sayHelloAsync(int delay,
                      function<void()> response,
                      function<void(exception_ptr)> error,
                      const Ice::Current&)
{
    if(delay == 0)
    {
        cout << "Hello World!" << endl;
        response();
    }
    else
    {
        _workQueue->add(delay, move(response), move(error));
    }
}

void
HelloI::shutdown(const Ice::Current& curr)
{
    cout << "Shutting down..." << endl;

    _workQueue->destroy();
    curr.adapter->getCommunicator()->shutdown();
}
