// **********************************************************************
//
// Copyright (c) 2003-2016 ZeroC, Inc. All rights reserved.
//
// **********************************************************************

#include <Ice/Ice.h>
#include <NestedI.h>

using namespace std;
using namespace Ice;
using namespace Demo;

NestedI::NestedI(const NestedPrx& self) :
    _self(self)
{
}

void
NestedI::nestedCall(Int level, const NestedPrx& proxy, const Ice::Current&)
{
    cout << level << endl;
    if(--level > 0)
    {
        //
        // Ensure the invocation times out if the nesting level is too
        // high and there are no more threads in the thread pool to
        // dispatch the call.
        //
        proxy->ice_invocationTimeout(5000)->nestedCall(level, _self);
    }
}
