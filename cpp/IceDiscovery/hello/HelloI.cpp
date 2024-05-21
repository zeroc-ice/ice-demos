//
// Copyright (c) ZeroC, Inc. All rights reserved.
//

#include <HelloI.h>
#include <Ice/Ice.h>
#include <thread>

using namespace std;

void
HelloI::sayHello(int delay, const Ice::Current&)
{
    if (delay != 0)
    {
        this_thread::sleep_for(chrono::milliseconds(delay));
    }
    cout << "Hello World!" << endl;
}

void
HelloI::shutdown(const Ice::Current& c)
{
    cout << "Shutting down..." << endl;
    c.adapter->getCommunicator()->shutdown();
}
