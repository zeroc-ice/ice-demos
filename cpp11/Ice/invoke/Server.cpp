// **********************************************************************
//
// Copyright (c) 2003-2016 ZeroC, Inc. All rights reserved.
//
// **********************************************************************

#include <Ice/Ice.h>
#include <PrinterI.h>

using namespace std;

class InvokeServer : public Ice::Application
{
public:

    virtual int run(int, char*[]);
};


int
main(int argc, char* argv[])
{
#ifdef ICE_STATIC_LIBS
    Ice::registerIceSSL();
#endif
    InvokeServer app;
    return app.main(argc, argv, "config.server");
}

int
InvokeServer::run(int argc, char*[])
{
    if(argc > 1)
    {
        cerr << appName() << ": too many arguments" << endl;
        return EXIT_FAILURE;
    }

    auto adapter = communicator()->createObjectAdapter("Printer");
    adapter->add(make_shared<PrinterI>(), Ice::stringToIdentity("printer"));
    adapter->activate();
    communicator()->waitForShutdown();
    return EXIT_SUCCESS;
}
