// **********************************************************************
//
// Copyright (c) 2003-present ZeroC, Inc. All rights reserved.
//
// **********************************************************************

#include <Ice/Ice.h>

using namespace std;

namespace
{

class LocatorRegistryI : public Ice::LocatorRegistry
{
public:

    LocatorRegistryI()
    {
    }

    virtual void
    setAdapterDirectProxy_async(const Ice::AMD_LocatorRegistry_setAdapterDirectProxyPtr& cb,
                                const string& id,
                                const Ice::ObjectPrx& proxy,
                                const Ice::Current&)
    {
        if(!proxy)
        {
            _adapters.erase(id);
        }
        else
        {
            _adapters[id] = proxy;
        }
        cb->ice_response();
    }

    virtual void
    setReplicatedAdapterDirectProxy_async(const Ice::AMD_LocatorRegistry_setReplicatedAdapterDirectProxyPtr& cb,
                                          const string&,
                                          const string&,
                                          const Ice::ObjectPrx&,
                                          const Ice::Current&)
    {
        assert(false); // Not used by this demo
        cb->ice_response();
    }

    virtual void
    setServerProcessProxy_async(const Ice::AMD_LocatorRegistry_setServerProcessProxyPtr& cb,
                                const string&,
                                const Ice::ProcessPrx&,
                                const Ice::Current&)
    {
        assert(false); // Not used by this demo
        cb->ice_response();
    }

    Ice::ObjectPrx
    getAdapter(const string& id)
    {
        map<string, Ice::ObjectPrx>::const_iterator p = _adapters.find(id);
        if(p == _adapters.end())
        {
            throw Ice::AdapterNotFoundException();
        }
        return p->second;
    }

private:

    map<string, Ice::ObjectPrx> _adapters;
};
typedef IceInternal::Handle<LocatorRegistryI> LocatorRegistryIPtr;

class LocatorI : public Ice::Locator
{
public:

    LocatorI(const LocatorRegistryIPtr& registry,
             const Ice::LocatorRegistryPrx& registryPrx) :
        _registry(registry),
        _registryPrx(registryPrx)
    {
    }

    virtual void
    findObjectById_async(const Ice::AMD_Locator_findObjectByIdPtr& cb,
                         const Ice::Identity&,
                         const Ice::Current&) const
    {
        cb->ice_response(0);
    }

    virtual void
    findAdapterById_async(const Ice::AMD_Locator_findAdapterByIdPtr& cb,
                          const string& id,
                          const Ice::Current&) const
    {
        cb->ice_response(_registry->getAdapter(id));
    }

    virtual Ice::LocatorRegistryPrx
    getRegistry(const Ice::Current&) const
    {
        return _registryPrx;
    }

private:

    // Required to prevent compiler warning with MSVC++
    LocatorI& operator=(const LocatorI&);

    const LocatorRegistryIPtr _registry;
    const Ice::LocatorRegistryPrx _registryPrx;
};

}

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
        Ice::CommunicatorHolder ich(argc, argv, "config.locator");
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
            Ice::ObjectAdapterPtr adapter = communicator->createObjectAdapter("Locator");
            LocatorRegistryIPtr registry = new LocatorRegistryI;
            Ice::LocatorRegistryPrx registryPrx =
                Ice::LocatorRegistryPrx::uncheckedCast(adapter->add(registry, Ice::stringToIdentity("registry")));
            Ice::LocatorPtr locator = new LocatorI(registry, registryPrx);
            adapter->add(locator, Ice::stringToIdentity("locator"));
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
