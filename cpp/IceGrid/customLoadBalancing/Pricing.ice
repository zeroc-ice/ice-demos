// **********************************************************************
//
// Copyright (c) 2003-2016 ZeroC, Inc. All rights reserved.
//
// **********************************************************************

#pragma once

#include <Ice/BuiltinSequences.ice>

module Demo
{

interface PricingEngine
{
    idempotent Ice::StringSeq getPreferredCurrencies();
};

};
