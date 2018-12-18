// **********************************************************************
//
// Copyright (c) 2003-present ZeroC, Inc. All rights reserved.
//
// **********************************************************************

#include <Ice/Ice.h>

using namespace std;

namespace
{

//
// AMI type-safe callbacks that forward to AMD callbacks

//
// Template for void response
//
template<class T>
class AMICallback : public IceUtil::Shared
{
public:

    AMICallback(const T& cb) :
        _cb(cb)
    {
    }

    void ice_response()
    {
        _cb->ice_response();
    }

    void ice_exception(const Ice::Exception& ex)
    {
        _cb->ice_exception(ex);
    }

private:

    // Required to prevent compiler warning with MSVC++
    AMICallback& operator=(const AMICallback&);

    const T _cb;
};

//
// Template for response = one proxy
//
template<class T>
class AMICallbackObjectPrx : public IceUtil::Shared
{
public:

    AMICallbackObjectPrx(const T& cb) :
        _cb(cb)
    {
    }

    void ice_response(const Ice::ObjectPrx& proxy)
    {
        _cb->ice_response(proxy);
    }

    void ice_exception(const Ice::Exception& ex)
    {
        _cb->ice_exception(ex);
    }

private:

    // Required to prevent compiler warning with MSVC++
    AMICallbackObjectPrx& operator=(const AMICallbackObjectPrx&);

    const T _cb;
};

//
// LocatorRegistry implementation
//
class LocatorRegistryI : public Ice::LocatorRegistry
{
public:

    LocatorRegistryI(const Ice::LocatorRegistryPrx& registry) :
        _registry(registry)
    {
    }

    virtual void
    setAdapterDirectProxy_async(const Ice::AMD_LocatorRegistry_setAdapterDirectProxyPtr& cb,
                                const string& id,
                                const Ice::ObjectPrx& proxy,
                                const Ice::Current&)
    {
        typedef AMICallback<Ice::AMD_LocatorRegistry_setAdapterDirectProxyPtr> SetAdapterDirectProxyCallback;
        typedef IceUtil::Handle<SetAdapterDirectProxyCallback> SetAdapterDirectProxyCallbackPtr;

        SetAdapterDirectProxyCallbackPtr callback = new SetAdapterDirectProxyCallback(cb);
        Ice::Callback_LocatorRegistry_setAdapterDirectProxyPtr d =
            Ice::newCallback_LocatorRegistry_setAdapterDirectProxy(callback,
                                                                   &SetAdapterDirectProxyCallback::ice_response,
                                                                   &SetAdapterDirectProxyCallback::ice_exception);
        _registry->begin_setAdapterDirectProxy(id, proxy, d);
    }

    virtual void
    setReplicatedAdapterDirectProxy_async(const Ice::AMD_LocatorRegistry_setReplicatedAdapterDirectProxyPtr& cb,
                                          const string& id,
                                          const string& group,
                                          const Ice::ObjectPrx& proxy,
                                          const Ice::Current&)
    {
        typedef AMICallback<Ice::AMD_LocatorRegistry_setReplicatedAdapterDirectProxyPtr> SetReplicatedAdapterDirectProxyCallback;
        typedef IceUtil::Handle<SetReplicatedAdapterDirectProxyCallback> SetReplicatedAdapterDirectProxyCallbackPtr;

        SetReplicatedAdapterDirectProxyCallbackPtr callback = new SetReplicatedAdapterDirectProxyCallback(cb);
        Ice::Callback_LocatorRegistry_setReplicatedAdapterDirectProxyPtr d =
            Ice::newCallback_LocatorRegistry_setReplicatedAdapterDirectProxy(callback,
                                                                             &SetReplicatedAdapterDirectProxyCallback::ice_response,
                                                                             &SetReplicatedAdapterDirectProxyCallback::ice_exception);
        _registry->begin_setReplicatedAdapterDirectProxy(id, group, proxy, d);
    }

    virtual void
    setServerProcessProxy_async(const Ice::AMD_LocatorRegistry_setServerProcessProxyPtr& cb,
                                const string& id,
                                const Ice::ProcessPrx& proxy,
                                const Ice::Current&)
    {
        typedef AMICallback<Ice::AMD_LocatorRegistry_setServerProcessProxyPtr> SetServerProcessProxyCallback;
        typedef IceUtil::Handle<SetServerProcessProxyCallback> SetServerProcessProxyCallbackPtr;

        SetServerProcessProxyCallbackPtr callback = new SetServerProcessProxyCallback(cb);
        Ice::Callback_LocatorRegistry_setServerProcessProxyPtr d =
            Ice::newCallback_LocatorRegistry_setServerProcessProxy(callback,
                                                                   &SetServerProcessProxyCallback::ice_response,
                                                                   &SetServerProcessProxyCallback::ice_exception);
        _registry->begin_setServerProcessProxy(id, proxy, d);
    }

private:

    // Required to prevent compiler warning with MSVC++
    LocatorRegistryI& operator=(const LocatorRegistryI&);

    const Ice::LocatorRegistryPrx _registry;
};

//
// Locator implementation
//
class LocatorI : public Ice::Locator
{
public:

    LocatorI(const Ice::LocatorPrx& locator,
             const Ice::LocatorRegistryPrx& registry) :
        _locator(locator),
        _registry(registry)
    {
    }

    virtual void
    findObjectById_async(const Ice::AMD_Locator_findObjectByIdPtr& cb,
                         const Ice::Identity& id,
                         const Ice::Current& curr) const
    {
        Ice::Context::const_iterator p = curr.ctx.find("SECRET");
        if(p != curr.ctx.end() && p->second == "LetMeIn")
        {
            typedef AMICallbackObjectPrx<Ice::AMD_Locator_findObjectByIdPtr> FindObjectByIdCallback;
            typedef IceUtil::Handle<FindObjectByIdCallback> FindObjectByIdCallbackPtr;

            FindObjectByIdCallbackPtr callback = new FindObjectByIdCallback(cb);
            Ice::Callback_Locator_findObjectByIdPtr d =
                Ice::newCallback_Locator_findObjectById(callback,
                                                        &FindObjectByIdCallback::ice_response,
                                                        &FindObjectByIdCallback::ice_exception);
            _locator->begin_findObjectById(id, d);
        }
        else
        {
            cb->ice_response(0);
        }
    }

    virtual void
    findAdapterById_async(const Ice::AMD_Locator_findAdapterByIdPtr& cb,
                          const string& id,
                          const Ice::Current& curr) const
    {
        Ice::Context::const_iterator p = curr.ctx.find("SECRET");
        if(p != curr.ctx.end() && p->second == "LetMeIn")
        {
            typedef AMICallbackObjectPrx<Ice::AMD_Locator_findAdapterByIdPtr> FindAdapterByIdCallback;
            typedef IceUtil::Handle<FindAdapterByIdCallback> FindAdapterByIdCallbackPtr;

            FindAdapterByIdCallbackPtr callback = new FindAdapterByIdCallback(cb);
            Ice::Callback_Locator_findAdapterByIdPtr d =
                Ice::newCallback_Locator_findAdapterById(callback,
                                                         &FindAdapterByIdCallback::ice_response,
                                                         &FindAdapterByIdCallback::ice_exception);
            _locator->begin_findAdapterById(id, d);
        }
        else
        {
            cb->ice_response(0);
        }
    }

    virtual Ice::LocatorRegistryPrx
    getRegistry(const Ice::Current&) const
    {
        return _registry;
    }

private:

    // Required to prevent compiler warning with MSVC++
    LocatorI& operator=(const LocatorI&);

    const Ice::LocatorPrx _locator;
    const Ice::LocatorRegistryPrx _registry;
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
            //
            // Get the IceGrid Locator and LocatorRegistry
            //
            Ice::LocatorPrx defaultLocator = communicator->getDefaultLocator();
            Ice::LocatorRegistryPrx defaultRegistry = defaultLocator->getRegistry();

            //
            // Create the custom Locator and LocatorRegistry
            //
            Ice::ObjectAdapterPtr adapter = communicator->createObjectAdapter("Locator");
            Ice::LocatorRegistryPtr registry = new LocatorRegistryI(defaultRegistry);
            Ice::LocatorRegistryPrx registryPrx =
                Ice::LocatorRegistryPrx::uncheckedCast(adapter->add(registry, Ice::stringToIdentity("registry")));
            Ice::LocatorPtr locator = new LocatorI(defaultLocator, registryPrx);
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
