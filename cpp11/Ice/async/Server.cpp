// **********************************************************************
//
// Copyright (c) 2003-2016 ZeroC, Inc. All rights reserved.
//
// **********************************************************************

#include <Ice/Ice.h>
#include <HelloI.h>
#include <WorkQueue.h>

using namespace std;

class AsyncServer : public Ice::Application
{
public:

    virtual int run(int, char*[]);
    virtual void interruptCallback(int);

private:

    shared_ptr<WorkQueue> _workQueue;
};

int
main(int argc, char* argv[])
{
    AsyncServer app;
    return app.main(argc, argv, "config.server");
}

int
AsyncServer::run(int argc, char*[])
{
    if(argc > 1)
    {
        cerr << appName() << ": too many arguments" << endl;
        return EXIT_FAILURE;
    }

    callbackOnInterrupt();

    auto adapter = communicator()->createObjectAdapter("Hello");
    _workQueue = make_shared<WorkQueue>();
    adapter->add(make_shared<HelloI>(_workQueue), Ice::stringToIdentity("hello"));

    _workQueue->start();
    adapter->activate();

    communicator()->waitForShutdown();
    _workQueue->join();
    return EXIT_SUCCESS;
}

void
AsyncServer::interruptCallback(int)
{
    _workQueue->destroy();
    communicator()->shutdown();
}
