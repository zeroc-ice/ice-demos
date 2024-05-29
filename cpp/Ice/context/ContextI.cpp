//
// Copyright (c) ZeroC, Inc. All rights reserved.
//

#include "ContextI.h"
#include <Ice/Ice.h>
#include <iostream>

using namespace std;

void
ContextI::call(const Ice::Current& c)
{
    cout << "Type = ";
    const auto p = c.ctx.find("type");
    cout << (p != c.ctx.end() ? p->second : "None") << endl;
}

void
ContextI::shutdown(const Ice::Current& c)
{
    cout << "Shutting down..." << endl;
    c.adapter->getCommunicator()->shutdown();
}
