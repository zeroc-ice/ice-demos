// **********************************************************************
//
// Copyright (c) 2003-present ZeroC, Inc. All rights reserved.
//
// **********************************************************************

#include <Ice/Ice.h>
#include <IceGrid/IceGrid.h>
#include <Pricing.h>

using namespace std;
using namespace Demo;

int run(const Ice::CommunicatorPtr&);

int
main(int argc, char* argv[])
{
#ifdef ICE_STATIC_LIBS
    Ice::registerIceLocatorDiscovery();
#endif

    int status = 0;

    try
    {
        //
        // CommunicatorHolder's ctor initializes an Ice communicator,
        // and its dtor destroys this communicator.
        //
        Ice::CommunicatorHolder ich(argc, argv, "config.client");

        //
        // The communicator initialization removes all Ice-related arguments from argc/argv
        //
        if(argc > 1)
        {
            cerr << argv[0] << ": too many arguments" << endl;
            status = 1;
        }
        else
        {
            status = run(ich.communicator());
        }
    }
    catch(const std::exception& ex)
    {
        cerr << argv[0] << ": " << ex.what() << endl;
        status = 1;
    }

    return status;
}

int
run(const Ice::CommunicatorPtr& communicator)
{
    //
    // Create a proxy to the well-known object with the `pricing'
    // identity.
    //
    PricingEnginePrx pricing = PricingEnginePrx::uncheckedCast(communicator->stringToProxy("pricing"));
    if(!pricing)
    {
        cerr << "couldn't find a `::Demo::PricingEngine' object." << endl;
        return 1;
    }

    //
    // If no context is set on the default locator (with the
    // Ice.Default.Locator.Context property, see the comments from the
    // `config.client' file in this directory), ask for the preferred
    // currency.
    //
    Ice::Context ctx = communicator->getDefaultLocator()->ice_getContext();
    if(ctx["currency"].empty())
    {
        cout << "enter your preferred currency (USD, EUR, GBP, INR, AUD, JPY): ";
        string currency;
        cin >> currency;

        //
        // Setup a locator proxy with a currency context.
        //
        Ice::LocatorPrx locator = communicator->getDefaultLocator();
        ctx["currency"] = currency;
        pricing = pricing->ice_locator(locator->ice_context(ctx));
    }
    else
    {
        cout << "Preferred currency configured for the client: " << ctx["currency"] << endl;
    }

    //
    // Get the preferred currencies of the server
    //
    Ice::StringSeq currencies = pricing->getPreferredCurrencies();
    cout << "Preferred currencies of the server: ";
    for(Ice::StringSeq::const_iterator p = currencies.begin(); p != currencies.end(); ++p)
    {
        cout << *p;
        if(p + 1 != currencies.end())
        {
            cout << ", ";
        }
    }
    cout << endl;
    return 0;
}
