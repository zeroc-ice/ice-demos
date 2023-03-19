//
// Copyright (c) ZeroC, Inc. All rights reserved.
//

#include <Ice/Ice.h>
#include <HelloI.h>
#include <thread>

using namespace std;

HelloI::HelloI(const function<void()>& requestShutdown) :
    _requestShutdown(requestShutdown)
{
}

void
HelloI::sayHello(int delay, const Ice::Current&)
{
    if(delay != 0)
    {
        this_thread::sleep_for(chrono::milliseconds(delay));
    }
    cout << "Hello World!" << endl;
}

void
HelloI::shutdown(const Ice::Current&)
{
    cout << "Shutting down..." << endl;
    _requestShutdown();
}
