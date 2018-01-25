// **********************************************************************
//
// Copyright (c) 2003-2018 ZeroC, Inc. All rights reserved.
//
// **********************************************************************

#include <Ice/Ice.h>
#include <HelloI.h>

using namespace std;

class Server : public Ice::Application
{
public:

    virtual int run(int argc, char* argv[]) override;

};

int
main(int argc, char* argv[])
{
    //
    // Set the 'BuildId' property displayed in the IceGridGUI
    //
    Ice::InitializationData initData;
    initData.properties = Ice::createProperties();
    initData.properties->setProperty("BuildId", string("Ice ") + ICE_STRING_VERSION);

    Server app;
    int status = app.main(argc, argv, initData);
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

    auto properties = communicator()->getProperties();
    auto adapter = communicator()->createObjectAdapter("Hello");
    auto id = Ice::stringToIdentity(properties->getProperty("Identity"));
    auto hello = make_shared<HelloI>(properties->getProperty("Ice.ProgramName"));
    adapter->add(hello, id);
    adapter->activate();
    communicator()->waitForShutdown();
    return EXIT_SUCCESS;
}
