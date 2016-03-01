// **********************************************************************
//
// Copyright (c) 2003-2016 ZeroC, Inc. All rights reserved.
//
// **********************************************************************

#include <Ice/Ice.h>
#include <PricingI.h>

using namespace std;

PricingI::PricingI(const Ice::StringSeq& currencies) : _currencies(currencies)
{
}

Ice::StringSeq
PricingI::getPreferredCurrencies(const Ice::Current&)
{
    return _currencies;
}
