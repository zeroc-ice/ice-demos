// **********************************************************************
//
// Copyright (c) 2003-2018 ZeroC, Inc. All rights reserved.
//
// **********************************************************************

#include <Ice/Ice.h>

using namespace std;

class PluginServer : public Ice::Application
{
public:

    virtual int
    run(int argc, char*[]) override
    {
        if(argc > 1)
        {
            cerr << appName() << ": too many arguments" << endl;
            return 1;
        }

        communicator()->waitForShutdown();
        return 0;
    }
};

int
main(int argc, char* argv[])
{
    PluginServer app;
    return app.main(argc, argv, "config.server");
}
