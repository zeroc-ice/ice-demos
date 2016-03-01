// **********************************************************************
//
// Copyright (c) 2003-2016 ZeroC, Inc. All rights reserved.
//
// **********************************************************************

#include <Ice/Ice.h>
#include <Parser.h>

using namespace std;

class LibraryClient : public Ice::Application
{
public:

    LibraryClient();

    virtual int run(int argc, char* argv[]);
};

int
main(int argc, char* argv[])
{
    LibraryClient app;
    return app.main(argc, argv, "config.client");
}

LibraryClient::LibraryClient() :
    //
    // Since this is an interactive demo we don't want any signal
    // handling.
    //
    Ice::Application(Ice::NoSignalHandling)
{
}

int
LibraryClient::run(int argc, char* argv[])
{
    int runParser(int, char*[], const Ice::CommunicatorPtr&);
    return runParser(argc, argv, communicator());
}
