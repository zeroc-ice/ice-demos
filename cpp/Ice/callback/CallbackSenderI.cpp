// **********************************************************************
//
// Copyright (c) 2003-2016 ZeroC, Inc. All rights reserved.
//
// **********************************************************************

#include <Ice/Ice.h>
#include <CallbackSenderI.h>

using namespace std;
using namespace Ice;
using namespace Demo;

void
CallbackSenderI::initiateCallback(const CallbackReceiverPrx& proxy, const Current&)
{
    cout << "initiating callback" << endl;
    try
    {
        proxy->callback();
    }
    catch(const Exception& ex)
    {
        cout << ex << endl;
    }
}

void
CallbackSenderI::shutdown(const Current& c)
{
    cout << "shutting down..." << endl;
    try
    {
        c.adapter->getCommunicator()->shutdown();
    }
    catch(const Exception& ex)
    {
        cout << ex << endl;
    }
}
