// **********************************************************************
//
// Copyright (c) 2003-2015 ZeroC, Inc. All rights reserved.
//
// **********************************************************************

#include <Ice/Ice.h>
#include <Props.h>

using namespace std;

//
// The servant implements the Slice interface Demo::Props as well as the
// native callback class Ice::PropertiesAdminUpdateCallback.
//
class PropsI : public Demo::Props,
               public Ice::PropertiesAdminUpdateCallback
{
public:

    PropsI() : _called(false)
    {
    }

    virtual Ice::PropertyDict getChanges(const Ice::Current&)
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

    virtual void shutdown(const Ice::Current& current)
    {
        current.adapter->getCommunicator()->shutdown();
    }

    virtual void updated(const Ice::PropertyDict& changes)
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

    virtual int run(int, char*[]);
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
    // Retrieve the PropertiesAdmin facet and register the servant as the update callback.
    //
    auto admin = dynamic_pointer_cast<Ice::NativePropertiesAdmin>(communicator()->findAdminFacet("Properties"));
    admin->addUpdateCallback(props);

    auto adapter = communicator()->createObjectAdapter("Props");
    adapter->add(props, communicator()->stringToIdentity("props"));
    adapter->activate();
    communicator()->waitForShutdown();
    return EXIT_SUCCESS;
}
