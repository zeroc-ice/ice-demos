// **********************************************************************
//
// Copyright (c) 2003-2017 ZeroC, Inc. All rights reserved.
//
// **********************************************************************

#include <Ice/Ice.h>
#include <PricingI.h>

using namespace std;

class Server : public Ice::Application
{
public:

    virtual int run(int argc, char* argv[]);
};

int
#ifdef _WIN32
wmain(int argc, wchar_t* argv[])
#else
main(int argc, char* argv[])
#endif
{
    Server app;
    int status = app.main(argc, argv);
    return status;
}

int
Server::run(int argc, char*[])
{
    if(argc > 1)
    {
        cerr << appName() << ": too many arguments" << endl;
        return EXIT_FAILURE;
    }

    Ice::PropertiesPtr properties = communicator()->getProperties();
    Ice::ObjectAdapterPtr adapter = communicator()->createObjectAdapter("Pricing");
    Ice::Identity id = Ice::stringToIdentity(properties->getProperty("Identity"));
    Demo::PricingEnginePtr pricing = new PricingI(properties->getPropertyAsList("Currencies"));
    adapter->add(pricing, id);
    adapter->activate();
    communicator()->waitForShutdown();
    return EXIT_SUCCESS;
}
