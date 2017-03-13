// **********************************************************************
//
// Copyright (c) 2003-2017 ZeroC, Inc. All rights reserved.
//
// **********************************************************************

#include <Ice/Ice.h>
#include <Hello.h>

using namespace std;
using namespace Demo;

int
#ifdef _WIN32
wmain(int argc, wchar_t* argv[])
#else
main(int argc, char* argv[])
#endif
{
    try
    {
        Ice::CommunicatorHolder ich(argc, argv);
        auto hello = Ice::checkedCast<HelloPrx>(ich->stringToProxy("hello:default -h localhost -p 10000"));
        hello->sayHello();
    }
    catch(const std::exception& ex)
    {
        cerr << ex.what() << endl;
        return 1;
    }
    return 0;
}
