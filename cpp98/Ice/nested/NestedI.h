// **********************************************************************
//
// Copyright (c) 2003-present ZeroC, Inc. All rights reserved.
//
// **********************************************************************

#ifndef NESTED_I_H
#define NESTED_I_H

#include <Nested.h>

class NestedI : public Demo::Nested
{
public:

    NestedI(const Demo::NestedPrx&);
    virtual void nestedCall(Ice::Int, const Demo::NestedPrx&, const Ice::Current&);

private:

    // Required to prevent compiler warnings with MSVC++
    NestedI& operator=(const NestedI&);

    const Demo::NestedPrx _self;
};

#endif
