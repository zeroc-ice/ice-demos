//
// Copyright (c) ZeroC, Inc. All rights reserved.
//

#include "HelloI.h"
#include <Ice/Ice.h>
#include <iostream>

using namespace std;

HelloI::HelloI(string_view name) : _name(name) {}

string
HelloI::getGreeting(const Ice::Current&)
{
    return _name + " says Hello World!";
}

void
HelloI::shutdown(const Ice::Current& current)
{
    cout << _name << " shutting down..." << endl;
    current.adapter->getCommunicator()->shutdown();
}
