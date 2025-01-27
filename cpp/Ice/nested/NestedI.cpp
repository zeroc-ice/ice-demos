// Copyright (c) ZeroC, Inc.

#include "NestedI.h"
#include <Ice/Ice.h>
#include <iostream>

using namespace std;
using namespace Demo;

NestedI::NestedI(optional<NestedPrx> self) : _self(std::move(self)) {}

void
NestedI::nestedCall(int level, optional<NestedPrx> proxy, const Ice::Current&)
{
    cout << level << endl;
    if (--level > 0)
    {
        //
        // Ensure the invocation times out if the nesting level is too
        // high and there are no more threads in the thread pool to
        // dispatch the call.
        //
        proxy->ice_invocationTimeout(5000)->nestedCall(level, _self);
    }
}
