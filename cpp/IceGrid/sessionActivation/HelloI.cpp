//
// Copyright (c) ZeroC, Inc. All rights reserved.
//

#include <HelloI.h>
#include <Ice/Ice.h>

using namespace std;

HelloI::HelloI(const string& name) : _name(name) {}

void
HelloI::sayHello(const Ice::Current&)
{
    cout << _name << " says Hello World!" << endl;
}
