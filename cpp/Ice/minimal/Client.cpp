//
// Copyright (c) ZeroC, Inc. All rights reserved.
//

#include "Hello.h"
#include <Ice/Ice.h>
#include <iostream>

using namespace std;
using namespace Demo;

int
main(int argc, char* argv[])
{
    try
    {
        const Ice::CommunicatorHolder ich(argc, argv);
        auto hello = Ice::checkedCast<HelloPrx>(ich->stringToProxy("hello:default -h localhost -p 10000"));
        hello->sayHello();
    }
    catch (const std::exception& ex)
    {
        cerr << ex.what() << endl;
        return 1;
    }
    return 0;
}
