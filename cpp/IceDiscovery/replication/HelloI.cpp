// Copyright (c) ZeroC, Inc.

#include "HelloI.h"
#include <Ice/Ice.h>
#include <iostream>

using namespace std;

HelloI::HelloI(string name) : _name(std::move(name)) {}

std::string
HelloI::getGreeting(const Ice::Current&)
{
    return _name + " says Hello World!";
}

void
HelloI::shutdown(const Ice::Current& c)
{
    cout << _name << " shutting down..." << endl;
    c.adapter->getCommunicator()->shutdown();
}
