// **********************************************************************
//
// Copyright (c) 2003-2016 ZeroC, Inc. All rights reserved.
//
// **********************************************************************

#ifndef PRICING_I_H
#define PRICING_I_H

#include <Pricing.h>

class PricingI : public Demo::PricingEngine
{
public:

    PricingI(const Ice::StringSeq&);

    virtual Ice::StringSeq getPreferredCurrencies(const Ice::Current&);

private:

	// Required to prevent compiler warnings with MSVC++
	PricingI& operator=(const PricingI&);

    const Ice::StringSeq _currencies;
};

#endif
