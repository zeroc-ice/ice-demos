//
// Copyright (c) ZeroC, Inc. All rights reserved.
//

#ifndef NESTED_I_H
#define NESTED_I_H

#include "Nested.h"

class NestedI final : public Demo::Nested
{
public:
    NestedI(std::optional<Demo::NestedPrx>);
    void nestedCall(int, std::optional<Demo::NestedPrx>, const Ice::Current&) final;

private:
    const std::optional<Demo::NestedPrx> _self;
};

#endif
