//
// Copyright (c) ZeroC, Inc. All rights reserved.
//

#include "HelloI.h"
#include <Ice/Ice.h>
#include <iostream>

using namespace std;

HelloI::HelloI(string_view name) : _name(name) {}

void
HelloI::sayHello(const Ice::Current&)
{
    cout << _name << " says Hello World!" << endl;
}
