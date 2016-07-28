// **********************************************************************
//
// Copyright (c) 2003-2016 ZeroC, Inc. All rights reserved.
//
// **********************************************************************

#include <Ice/Ice.h>
#include <Props.h>

using namespace std;

//
// The servant implements the Slice interface Demo::Props
//
class PropsI : public Demo::Props
{
public:

    PropsI() : _called(false)
    {
    }

    virtual Ice::PropertyDict getChanges(const Ice::Current&) override
    {
        unique_lock<mutex> lock(_mutex);

        //
        // Make sure that we have received the property updates before we
        // return the results.
        //
        while(!_called)
        {
            _cv.wait(lock);
        }

        _called = false;
        return _changes;
    }

    virtual void shutdown(const Ice::Current& current) override
    {
        current.adapter->getCommunicator()->shutdown();
    }

    void updated(const Ice::PropertyDict& changes)
    {
        unique_lock<mutex> lock(_mutex);

        _changes = changes;
        _called = true;
        _cv.notify_one();
    }

private:

    Ice::PropertyDict _changes;
    bool _called;
    mutex _mutex;
    condition_variable _cv;
};

class Server : public Ice::Application
{
public:

    virtual int run(int, char*[]) override;
};


int
main(int argc, char* argv[])
{
#ifdef ICE_STATIC_LIBS
    Ice::registerIceSSL();
#endif
    Server app;
    return app.main(argc, argv, "config.server");
}

int
Server::run(int argc, char*[])
{
    if(argc > 1)
    {
        cerr << appName() << ": too many arguments" << endl;
        return EXIT_FAILURE;
    }

    auto props = make_shared<PropsI>();

    //
    // Retrieve the PropertiesAdmin facet and register the update callback.
    //
    auto admin = dynamic_pointer_cast<Ice::NativePropertiesAdmin>(communicator()->findAdminFacet("Properties"));
    admin->addUpdateCallback([props](const Ice::PropertyDict& changes) { props->updated(changes); });

    auto adapter = communicator()->createObjectAdapter("Props");
    adapter->add(props, Ice::stringToIdentity("props"));
    adapter->activate();
    communicator()->waitForShutdown();
    return EXIT_SUCCESS;
}
