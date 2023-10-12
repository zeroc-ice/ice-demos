//
// Copyright (c) ZeroC, Inc. All rights reserved.
//

#include <Ice/Ice.h>
#include <Printer.h>

using namespace std;

void
Printer::sayHello(const Ice::Current&)
{
    cout << "Hello World!" << endl;
}
