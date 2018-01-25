// **********************************************************************
//
// Copyright (c) 2003-2018 ZeroC, Inc. All rights reserved.
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
        Ice::CommunicatorHolder ich(argc, argv);
        auto adapter =
            ich->createObjectAdapterWithEndpoints("Hello", "default -h localhost -p 10000");
        adapter->add(make_shared<HelloI>(), Ice::stringToIdentity("hello"));
        adapter->activate();
        ich->waitForShutdown();
    }
    catch(const std::exception& ex)
    {
        cerr << ex.what() << endl;
        return 1;
    }
    return 0;
}
