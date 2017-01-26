// **********************************************************************
//
// Copyright (c) 2003-2016 ZeroC, Inc. All rights reserved.
//
// **********************************************************************

#include <Ice/Ice.h>
#include <HelloI.h>

using namespace std;

class Server : public Ice::Application
{
public:

    virtual int run(int argc, char* argv[]);

};

#ifdef ICE_STATIC_LIBS
extern "C"
{

Ice::Plugin* createIceSSL(const Ice::CommunicatorPtr&, const string&, const Ice::StringSeq&);
Ice::Plugin* createIceDiscovery(const Ice::CommunicatorPtr&, const string&, const Ice::StringSeq&);

}
#endif

int
main(int argc, char* argv[])
{
#ifdef ICE_STATIC_LIBS
    Ice::registerPluginFactory("IceSSL", createIceSSL, true);
    Ice::registerPluginFactory("IceDiscovery", createIceDiscovery, false);
#endif
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
    Ice::ObjectAdapterPtr adapter = communicator()->createObjectAdapter("Hello");
    Ice::Identity id = Ice::stringToIdentity("hello");
    Demo::HelloPtr hello = new HelloI(properties->getProperty("Ice.ProgramName"));
    adapter->add(hello, id);
    adapter->activate();
    communicator()->waitForShutdown();
    return EXIT_SUCCESS;
}
