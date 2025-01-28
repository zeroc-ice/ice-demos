// Copyright (c) ZeroC, Inc.

#include "CallbackI.h"
#include <Ice/Ice.h>
#include <iostream>

using namespace std;
using namespace Demo;

void
CallbackI::initiateCallback(optional<CallbackReceiverPrx> proxy, const Ice::Current& current)
{
    cout << "initiating callback to: " << current.adapter->getCommunicator()->proxyToString(proxy) << endl;
    try
    {
        proxy->callback();
    }
    catch (const Ice::Exception& ex)
    {
        cout << ex << endl;
    }
}

void
CallbackI::shutdown(const Ice::Current& c)
{
    cout << "shutting down..." << endl;
    c.adapter->getCommunicator()->shutdown();
}
