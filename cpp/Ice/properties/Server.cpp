// Copyright (c) ZeroC, Inc.

#include "Props.h"
#include <Ice/Ice.h>
#include <iostream>

using namespace std;

//
// The servant implements the Slice interface Demo::Props
//
class PropsI final : public Demo::Props
{
public:
    Ice::PropertyDict getChanges(const Ice::Current&) final
    {
        unique_lock<mutex> lock(_mutex);

        //
        // Make sure that we have received the property updates before we
        // return the results.
        //
        while (!_called)
        {
            _cv.wait(lock);
        }

        _called = false;
        return _changes;
    }

    void shutdown(const Ice::Current& current) final { current.adapter->getCommunicator()->shutdown(); }

    void updated(const Ice::PropertyDict& changes)
    {
        const lock_guard<mutex> lock(_mutex);

        _changes = changes;
        _called = true;
        _cv.notify_one();
    }

private:
    Ice::PropertyDict _changes;
    bool _called = false;
    mutex _mutex;
    condition_variable _cv;
};

int
main(int argc, char* argv[])
{
    int status = 0;

    try
    {
        //
        // CtrlCHandler must be created before the communicator or any other threads are started
        //
        Ice::CtrlCHandler ctrlCHandler;

        //
        // CommunicatorHolder's ctor initializes an Ice communicator,
        // and its dtor destroys this communicator.
        //
        const Ice::CommunicatorHolder ich(argc, argv, "config.server");
        const auto& communicator = ich.communicator();

        ctrlCHandler.setCallback([communicator](int) { communicator->shutdown(); });

        //
        // The communicator initialization removes all Ice-related arguments from argc/argv
        //
        if (argc > 1)
        {
            cerr << argv[0] << ": too many arguments" << endl;
            status = 1;
        }
        else
        {
            auto props = make_shared<PropsI>();

            //
            // Retrieve the PropertiesAdmin facet and register the update callback.
            //
            auto admin = dynamic_pointer_cast<Ice::NativePropertiesAdmin>(communicator->findAdminFacet("Properties"));
            admin->addUpdateCallback([props](const Ice::PropertyDict& changes) { props->updated(changes); });

            auto adapter = communicator->createObjectAdapter("Props");
            adapter->add(props, Ice::stringToIdentity("props"));
            adapter->activate();

            communicator->waitForShutdown();
        }
    }
    catch (const std::exception& ex)
    {
        cerr << ex.what() << endl;
        status = 1;
    }

    return status;
}
