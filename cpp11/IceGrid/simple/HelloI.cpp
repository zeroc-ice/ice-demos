// **********************************************************************
//
// Copyright (c) 2003-present ZeroC, Inc. All rights reserved.
//
// **********************************************************************

#include <Ice/Ice.h>
#include <HelloI.h>

using namespace std;

HelloI::HelloI(const string& name) :
    _name(name)
{
}

void
HelloI::sayHello(const Ice::Current&)
{
    cout << _name << " says Hello World!" << endl;
}

void
HelloI::shutdown(const Ice::Current& current)
{
    cout << _name << " shutting down..." << endl;
    current.adapter->getCommunicator()->shutdown();
}
