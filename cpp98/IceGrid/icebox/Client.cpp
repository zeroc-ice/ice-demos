// **********************************************************************
//
// Copyright (c) 2003-2018 ZeroC, Inc. All rights reserved.
//
// **********************************************************************

#include <Ice/Ice.h>
#include <Hello.h>

using namespace std;
using namespace Demo;

class HelloClient : public Ice::Application
{
public:

    virtual int run(int, char*[]);
};

int
main(int argc, char* argv[])
{
#ifdef ICE_STATIC_LIBS
    Ice::registerIceLocatorDiscovery();
#endif

    HelloClient app;
    return app.main(argc, argv, "config.client");
}

int
HelloClient::run(int argc, char* argv[])
{
    if(argc > 1)
    {
        cerr << appName() << ": too many arguments" << endl;
        return 1;
    }

    HelloPrx hello = HelloPrx::uncheckedCast(communicator()->propertyToProxy("Hello.Proxy"));
    if(!hello)
    {
        cerr << argv[0] << ": invalid or missing Hello.Proxy property" << endl;
        return 1;
    }

    hello->sayHello();

    return 0;
}
