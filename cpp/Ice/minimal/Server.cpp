// **********************************************************************
//
// Copyright (c) 2003-2016 ZeroC, Inc. All rights reserved.
//
// **********************************************************************

#include <Ice/Ice.h>
#include <HelloI.h>

using namespace std;

int
main(int argc, char* argv[])
{
    try
    {
        Ice::CommunicatorHolder icHolder = Ice::initialize(argc, argv);
        Ice::ObjectAdapterPtr adapter =
            icHolder->createObjectAdapterWithEndpoints("Hello", "default -h localhost -p 10000");
        adapter->add(new HelloI, Ice::stringToIdentity("hello"));
        adapter->activate();
        icHolder->waitForShutdown();
    }
    catch(const std::exception& ex)
    {
        cerr << ex.what() << endl;
        return 1;
    }
    return 0;
}
