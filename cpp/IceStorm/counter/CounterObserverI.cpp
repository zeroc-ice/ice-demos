//
// Copyright (c) ZeroC, Inc. All rights reserved.
//

#include "CounterObserverI.h"
#include <Ice/Ice.h>
#include <iostream>

using namespace std;

void
print(const string& str)
{
    static mutex _coutMutex;
    const lock_guard<mutex> sync(_coutMutex);
    cout << str << flush;
}

void
CounterObserverI::init(int value, const Ice::Current&)
{
    const lock_guard<mutex> sync(_mutex);
    _value = value;
    ostringstream ostr;
    ostr << "init: " << value << endl;
    print(ostr.str());
}

void
CounterObserverI::inc(int value, const Ice::Current&)
{
    const lock_guard<mutex> sync(_mutex);
    _value += value;
    ostringstream ostr;
    ostr << "int: " << value << " total: " << _value << endl;
    print(ostr.str());
}
