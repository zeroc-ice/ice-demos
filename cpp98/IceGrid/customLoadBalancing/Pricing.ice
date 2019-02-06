//
// Copyright (c) ZeroC, Inc. All rights reserved.
//

#pragma once

#include <Ice/BuiltinSequences.ice>

module Demo
{
    interface PricingEngine
    {
        idempotent Ice::StringSeq getPreferredCurrencies();
    }
}
