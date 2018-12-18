// **********************************************************************
//
// Copyright (c) 2003-present ZeroC, Inc. All rights reserved.
//
// **********************************************************************

#include <Ice/Ice.h>
#include <CallbackSenderI.h>

using namespace std;
using namespace Demo;

void
CallbackSenderI::initiateCallback(shared_ptr<CallbackReceiverPrx> proxy, const Ice::Current&)
{
    cout << "initiating callback" << endl;
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
CallbackSenderI::shutdown(const Ice::Current& c)
{
    cout << "shutting down..." << endl;
    c.adapter->getCommunicator()->shutdown();
}
