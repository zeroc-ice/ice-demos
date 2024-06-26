//
// Copyright (c) ZeroC, Inc. All rights reserved.
//

#include "CallbackSenderI.h"
#include <Ice/Ice.h>
#include <iostream>

using namespace std;
using namespace Demo;

void
CallbackSenderI::initiateCallback(optional<CallbackReceiverPrx> proxy, const Ice::Current&)
{
    cout << "initiating callback" << endl;
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
CallbackSenderI::shutdown(const Ice::Current& c)
{
    cout << "shutting down..." << endl;
    c.adapter->getCommunicator()->shutdown();
}
