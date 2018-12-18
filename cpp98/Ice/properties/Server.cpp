// **********************************************************************
//
// Copyright (c) 2003-present ZeroC, Inc. All rights reserved.
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
               public Ice::PropertiesAdminUpdateCallback,
               IceUtil::Monitor<IceUtil::Mutex>
{
public:

    PropsI() : _called(false)
    {
    }

    virtual Ice::PropertyDict getChanges(const Ice::Current&)
    {
        IceUtil::Monitor<IceUtil::Mutex>::Lock sync(*this);

        //
        // Make sure that we have received the property updates before we
        // return the results.
        //
        while(!_called)
        {
            wait();
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
        IceUtil::Monitor<IceUtil::Mutex>::Lock sync(*this);

        _changes = changes;
        _called = true;
        notify();
    }

private:

    Ice::PropertyDict _changes;
    bool _called;
};
typedef IceUtil::Handle<PropsI> PropsIPtr;

//
// Global variable for shutdownCommunicator
//
Ice::CommunicatorPtr communicator;

//
// Callback for CtrlCHandler
//
void
shutdownCommunicator(int)
{
    communicator->shutdown();
}

int
main(int argc, char* argv[])
{
#ifdef ICE_STATIC_LIBS
    Ice::registerIceSSL();
#endif
    int status = 0;

    try
    {
        //
        // CtrlCHandler must be created before the communicator or any other threads are started
        //
        Ice::CtrlCHandler ctrlCHandler;

        //
        // CommunicatorHolder's ctor initializes an Ice communicator,
        // and it's dtor destroys this communicator.
        //
        Ice::CommunicatorHolder ich(argc, argv, "config.server");
        communicator = ich.communicator();

        //
        // Shutdown communicator on Ctrl-C
        //
        ctrlCHandler.setCallback(&shutdownCommunicator);

        //
        // The communicator initialization removes all Ice-related arguments from argc/argv
        //
        if(argc > 1)
        {
            cerr << argv[0] << ": too many arguments" << endl;
            status = 1;
        }
        else
        {
            PropsIPtr props = new PropsI;

            //
            // Retrieve the PropertiesAdmin facet and register the servant as the update callback.
            //
            Ice::ObjectPtr obj = communicator->findAdminFacet("Properties");
            Ice::NativePropertiesAdminPtr admin = Ice::NativePropertiesAdminPtr::dynamicCast(obj);
            admin->addUpdateCallback(props);

            Ice::ObjectAdapterPtr adapter = communicator->createObjectAdapter("Props");
            adapter->add(props, Ice::stringToIdentity("props"));
            adapter->activate();

            communicator->waitForShutdown();
        }
    }
    catch(const std::exception& ex)
    {
        cerr << ex.what() << endl;
        status = 1;
    }

    return status;
}
