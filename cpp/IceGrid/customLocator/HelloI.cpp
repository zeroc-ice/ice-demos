//
// Copyright (c) ZeroC, Inc. All rights reserved.
//

#include <HelloI.h>
#include <Ice/Ice.h>

using namespace std;

HelloI::HelloI(string name) : _name(std::move(name)) {}

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
