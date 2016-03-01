// **********************************************************************
//
// Copyright (c) 2003-2016 ZeroC, Inc. All rights reserved.
//
// **********************************************************************

#include <IceUtil/IceUtil.h>
#include <Ice/Ice.h>
#include <ContextI.h>

using namespace std;

void
ContextI::call(const Ice::Current& c)
{
    cout << "Type = ";
    Ice::Context::const_iterator p = c.ctx.find("type");
    if (p != c.ctx.end())
    {
    	cout << p->second;
    }
    else
    {
    	cout << "None";
    }
    cout << endl;
}

void
ContextI::shutdown(const Ice::Current& c)
{
    cout << "Shutting down..." << endl;
    c.adapter->getCommunicator()->shutdown();
}
