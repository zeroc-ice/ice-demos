// **********************************************************************
//
// Copyright (c) 2003-2017 ZeroC, Inc. All rights reserved.
//
// **********************************************************************

#pragma once

#include <Ice/BuiltinSequences.ice>

module Demo
{
    interface PricingEngine
    {
        idempotent Ice::StringSeq getPreferredCurrencies();
    }
}
