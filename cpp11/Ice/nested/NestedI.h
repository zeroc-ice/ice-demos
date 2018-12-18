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

    NestedI(std::shared_ptr<Demo::NestedPrx>);
    virtual void nestedCall(int, std::shared_ptr<Demo::NestedPrx>, const Ice::Current&) override;

private:

    const std::shared_ptr<Demo::NestedPrx> _self;
};

#endif
