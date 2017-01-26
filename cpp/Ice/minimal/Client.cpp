// **********************************************************************
//
// Copyright (c) 2003-2016 ZeroC, Inc. All rights reserved.
//
// **********************************************************************

#include <Ice/Ice.h>
#include <Hello.h>

using namespace std;
using namespace Demo;

int
main(int argc, char* argv[])
{  
    try
    {
        Ice::CommunicatorHolder icHolder = Ice::initialize(argc, argv);
        HelloPrx hello = HelloPrx::checkedCast(icHolder->stringToProxy("hello:default -h localhost -p 10000"));
        hello->sayHello();
    }
    catch(const std::exception& ex)
    {
        cerr << ex.what() << endl;
        return 1;
    }
    return 0;
}
