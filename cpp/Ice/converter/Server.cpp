// **********************************************************************
//
// Copyright (c) 2003-2016 ZeroC, Inc. All rights reserved.
//
// **********************************************************************

#include <Ice/Ice.h>
#include <GreetI.h>

using namespace std;

class ConverterServer : public Ice::Application
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
    ConverterServer app;
    return app.main(argc, argv, "config.server");
}

int
ConverterServer::run(int argc, char*[])
{
    if(argc > 1)
    {
        cerr << appName() << ": too many arguments" << endl;
        return EXIT_FAILURE;
    }

    Ice::ObjectAdapterPtr adapter = communicator()->createObjectAdapter("Greet");
    Demo::GreetPtr greet = new GreetI;
    adapter->add(greet, Ice::stringToIdentity("greet"));
    adapter->activate();
    communicator()->waitForShutdown();
    return EXIT_SUCCESS;
}
