//
// Copyright (c) ZeroC, Inc. All rights reserved.
//

#include "HelloI.h"
#include <Ice/Ice.h>
#include <iostream>

using namespace std;

void
HelloI::sayHello(const Ice::Current&)
{
    cout << "Hello World!" << endl;
}
