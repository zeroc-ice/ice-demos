// **********************************************************************
//
// Copyright (c) 2003-present ZeroC, Inc. All rights reserved.
//
// **********************************************************************

#include <Ice/Ice.h>
#include <CallbackI.h>

using namespace std;
using namespace Demo;

void
CallbackReceiverI::callback(const Ice::Current&)
{
    cout << "received callback" << endl;
}

void
CallbackI::initiateCallback(shared_ptr<CallbackReceiverPrx> proxy, const Ice::Current& current)
{
    cout << "initiating callback to: " << current.adapter->getCommunicator()->proxyToString(proxy) << endl;
    try
    {
        proxy->callback();
    }
    catch(const Ice::Exception& ex)
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
