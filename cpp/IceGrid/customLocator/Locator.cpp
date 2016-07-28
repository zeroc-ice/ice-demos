// **********************************************************************
//
// Copyright (c) 2003-2016 ZeroC, Inc. All rights reserved.
//
// **********************************************************************

#include <Ice/Ice.h>

using namespace std;

namespace
{

class SetAdapterDirectProxyCallback : public IceUtil::Shared
{
public:

    SetAdapterDirectProxyCallback(const Ice::AMD_LocatorRegistry_setAdapterDirectProxyPtr& cb) :
        _cb(cb)
    {
    }

    void
    finished(const Ice::AsyncResultPtr& r)
    {
        Ice::LocatorRegistryPrx registry = Ice::LocatorRegistryPrx::uncheckedCast(r->getProxy());
        try
        {
            registry->end_setAdapterDirectProxy(r);
            _cb->ice_response();
        }
        catch(const Ice::Exception& ex)
        {
            _cb->ice_exception(ex);
        }
    }

private:

	// Required to prevent compiler warning with MSVC++
	SetAdapterDirectProxyCallback& operator=(const SetAdapterDirectProxyCallback&);

    const Ice::AMD_LocatorRegistry_setAdapterDirectProxyPtr _cb;
};
typedef IceUtil::Handle<SetAdapterDirectProxyCallback> SetAdapterDirectProxyCallbackPtr;

class SetReplicatedAdapterDirectProxyCallback : public IceUtil::Shared
{
public:

    SetReplicatedAdapterDirectProxyCallback(const Ice::AMD_LocatorRegistry_setReplicatedAdapterDirectProxyPtr& cb) :
        _cb(cb)
    {
    }

    void
    finished(const Ice::AsyncResultPtr& r)
    {
        Ice::LocatorRegistryPrx registry = Ice::LocatorRegistryPrx::uncheckedCast(r->getProxy());
        try
        {
            registry->end_setReplicatedAdapterDirectProxy(r);
            _cb->ice_response();
        }
        catch(const Ice::Exception& ex)
        {
            _cb->ice_exception(ex);
        }
    }

private:

	// Required to prevent compiler warning with MSVC++
	SetReplicatedAdapterDirectProxyCallback& operator=(const SetReplicatedAdapterDirectProxyCallback&);

    const Ice::AMD_LocatorRegistry_setReplicatedAdapterDirectProxyPtr _cb;
};
typedef IceUtil::Handle<SetReplicatedAdapterDirectProxyCallback> SetReplicatedAdapterDirectProxyCallbackPtr;

class SetServerProcessProxyCallback : public IceUtil::Shared
{
public:

    SetServerProcessProxyCallback(const Ice::AMD_LocatorRegistry_setServerProcessProxyPtr& cb) :
        _cb(cb)
    {
    }

    void
    finished(const Ice::AsyncResultPtr& r)
    {
        Ice::LocatorRegistryPrx registry = Ice::LocatorRegistryPrx::uncheckedCast(r->getProxy());
        try
        {
            registry->end_setServerProcessProxy(r);
            _cb->ice_response();
        }
        catch(const Ice::Exception& ex)
        {
            _cb->ice_exception(ex);
        }
    }

private:

	// Required to prevent compiler warning with MSVC++
	SetServerProcessProxyCallback& operator=(const SetServerProcessProxyCallback&);

    const Ice::AMD_LocatorRegistry_setServerProcessProxyPtr _cb;
};
typedef IceUtil::Handle<SetServerProcessProxyCallback> SetServerProcessProxyCallbackPtr;

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
        SetAdapterDirectProxyCallbackPtr callback = new SetAdapterDirectProxyCallback(cb);
        Ice::CallbackPtr d = Ice::newCallback(callback, &SetAdapterDirectProxyCallback::finished);
        _registry->begin_setAdapterDirectProxy(id, proxy, d);
    }

    virtual void
    setReplicatedAdapterDirectProxy_async(const Ice::AMD_LocatorRegistry_setReplicatedAdapterDirectProxyPtr& cb,
                                          const string& id,
                                          const string& group,
                                          const Ice::ObjectPrx& proxy,
                                          const Ice::Current&)
    {
        SetReplicatedAdapterDirectProxyCallbackPtr callback = new SetReplicatedAdapterDirectProxyCallback(cb);
        Ice::CallbackPtr d = Ice::newCallback(callback, &SetReplicatedAdapterDirectProxyCallback::finished);
        _registry->begin_setReplicatedAdapterDirectProxy(id, group, proxy, d);
    }

    virtual void
    setServerProcessProxy_async(const Ice::AMD_LocatorRegistry_setServerProcessProxyPtr& cb,
                                const string& id,
                                const Ice::ProcessPrx& proxy,
                                const Ice::Current&)
    {
        SetServerProcessProxyCallbackPtr callback = new SetServerProcessProxyCallback(cb);
        Ice::CallbackPtr d = Ice::newCallback(callback, &SetServerProcessProxyCallback::finished);
        _registry->begin_setServerProcessProxy(id, proxy, d);
    }

private:

	// Required to prevent compiler warning with MSVC++
	LocatorRegistryI& operator=(const LocatorRegistryI&);

    const Ice::LocatorRegistryPrx _registry;
};

class FindObjectByIdCallback : public IceUtil::Shared
{
public:

    FindObjectByIdCallback(const Ice::AMD_Locator_findObjectByIdPtr& cb) :
        _cb(cb)
    {
    }

    void
    finished(const Ice::AsyncResultPtr& r)
    {
        Ice::LocatorPrx locator = Ice::LocatorPrx::uncheckedCast(r->getProxy());
        try
        {
            Ice::ObjectPrx obj = locator->end_findObjectById(r);
            _cb->ice_response(obj);
        }
        catch(const Ice::Exception& ex)
        {
            _cb->ice_exception(ex);
        }
    }

private:

	// Required to prevent compiler warning with MSVC++
	FindObjectByIdCallback& operator=(const FindObjectByIdCallback&);

    const Ice::AMD_Locator_findObjectByIdPtr _cb;
};
typedef IceUtil::Handle<FindObjectByIdCallback> FindObjectByIdCallbackPtr;

class FindAdapterByIdCallback : public IceUtil::Shared
{
public:

    FindAdapterByIdCallback(const Ice::AMD_Locator_findAdapterByIdPtr& cb) :
        _cb(cb)
    {
    }

    void
    finished(const Ice::AsyncResultPtr& r)
    {
        Ice::LocatorPrx locator = Ice::LocatorPrx::uncheckedCast(r->getProxy());
        try
        {
            Ice::ObjectPrx obj = locator->end_findAdapterById(r);
            _cb->ice_response(obj);
        }
        catch(const Ice::Exception& ex)
        {
            _cb->ice_exception(ex);
        }
    }

private:

	// Required to prevent compiler warning with MSVC++
	FindAdapterByIdCallback& operator=(const FindAdapterByIdCallback&);

    const Ice::AMD_Locator_findAdapterByIdPtr _cb;
};
typedef IceUtil::Handle<FindAdapterByIdCallback> FindAdapterByIdCallbackPtr;

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
            FindObjectByIdCallbackPtr callback = new FindObjectByIdCallback(cb);
            Ice::CallbackPtr d = Ice::newCallback(callback, &FindObjectByIdCallback::finished);
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
            FindAdapterByIdCallbackPtr callback = new FindAdapterByIdCallback(cb);
            Ice::CallbackPtr d = Ice::newCallback(callback, &FindAdapterByIdCallback::finished);
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

class LocatorServer : public Ice::Application
{


    virtual int run(int, char*[]);
};

int
main(int argc, char* argv[])
{
    LocatorServer app;
    return app.main(argc, argv, "config.locator");
}

int
LocatorServer::run(int argc, char*[])
{
    if(argc > 1)
    {
        cerr << appName() << ": too many arguments" << endl;
        return EXIT_FAILURE;
    }

    //
    // Get the IceGrid Locator and LocatorRegistry
    //
    Ice::LocatorPrx defaultLocator = communicator()->getDefaultLocator();
    Ice::LocatorRegistryPrx defaultRegistry = defaultLocator->getRegistry();

    //
    // Create the custom Locator and LocatorRegistry
    //
    Ice::ObjectAdapterPtr adapter = communicator()->createObjectAdapter("Locator");
    Ice::LocatorRegistryPtr registry = new LocatorRegistryI(defaultRegistry);
    Ice::LocatorRegistryPrx registryPrx =
        Ice::LocatorRegistryPrx::uncheckedCast(adapter->add(registry, Ice::stringToIdentity("registry")));
    Ice::LocatorPtr locator = new LocatorI(defaultLocator, registryPrx);
    adapter->add(locator, Ice::stringToIdentity("locator"));
    adapter->activate();
    communicator()->waitForShutdown();
    return EXIT_SUCCESS;
}
