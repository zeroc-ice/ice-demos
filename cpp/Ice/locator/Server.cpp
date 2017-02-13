// **********************************************************************
//
// Copyright (c) 2003-2017 ZeroC, Inc. All rights reserved.
//
// **********************************************************************

#include <Ice/Ice.h>
#include <HelloI.h>

using namespace std;

class HelloServer : public Ice::Application
{
public:

    virtual int run(int, char*[]);
};

int
#ifdef _WIN32
wmain(int argc, wchar_t* argv[])
#else
main(int argc, char* argv[])
#endif
{
    HelloServer app;
    return app.main(argc, argv, "config.server");
}

int
HelloServer::run(int argc, char*[])
{
    if(argc > 1)
    {
        cerr << appName() << ": too many arguments" << endl;
        return EXIT_FAILURE;
    }

    Ice::ObjectAdapterPtr adapter = communicator()->createObjectAdapter("Hello");
    Demo::HelloPtr hello = new HelloI;
    adapter->add(hello, Ice::stringToIdentity("hello"));
    adapter->activate();
    communicator()->waitForShutdown();
    return EXIT_SUCCESS;
}
