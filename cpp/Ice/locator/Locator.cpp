//
// Copyright (c) ZeroC, Inc. All rights reserved.
//

#include <Ice/Ice.h>
#include <iostream>

using namespace std;

namespace
{

    class LocatorRegistryI final : public Ice::LocatorRegistry
    {
    public:
        void setAdapterDirectProxyAsync(
            string id,
            optional<Ice::ObjectPrx> proxy,
            function<void()> response,
            function<void(exception_ptr)>,
            const Ice::Current&) final
        {
            if (proxy)
            {
                _adapters.insert({id, std::move(*proxy)});
            }
            else
            {
                _adapters.erase(id);
            }
            response();
        }

        void setReplicatedAdapterDirectProxyAsync(
            string,
            string,
            optional<Ice::ObjectPrx>,
            function<void()> response,
            function<void(exception_ptr)>,
            const Ice::Current&) final
        {
            assert(false); // Not used by this demo
            response();
        }

        void setServerProcessProxyAsync(
            string,
            optional<Ice::ProcessPrx>,
            function<void()> response,
            function<void(exception_ptr)>,
            const Ice::Current&) final
        {
            assert(false); // Not used by this demo
            response();
        }

        optional<Ice::ObjectPrx> getAdapter(const string& id)
        {
            auto p = _adapters.find(id);
            if (p == _adapters.end())
            {
                throw Ice::AdapterNotFoundException();
            }
            return p->second;
        }

    private:
        map<string, Ice::ObjectPrx> _adapters;
    };

    class LocatorI final : public Ice::Locator
    {
    public:
        LocatorI(shared_ptr<LocatorRegistryI> registry, optional<Ice::LocatorRegistryPrx> registryPrx)
            : _registry(std::move(registry)),
              _registryPrx(std::move(registryPrx))
        {
        }

        void findObjectByIdAsync(
            Ice::Identity,
            function<void(const optional<Ice::ObjectPrx>&)> response,
            function<void(exception_ptr)>,
            const Ice::Current&) const final
        {
            response(nullopt);
        }

        void findAdapterByIdAsync(
            string id,
            function<void(const optional<Ice::ObjectPrx>&)> response,
            function<void(exception_ptr)>,
            const Ice::Current&) const final
        {
            response(_registry->getAdapter(id));
        }

        optional<Ice::LocatorRegistryPrx> getRegistry(const Ice::Current&) const final { return _registryPrx; }

    private:
        const shared_ptr<LocatorRegistryI> _registry;
        const optional<Ice::LocatorRegistryPrx> _registryPrx;
    };

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
        // and its dtor destroys this communicator.
        //
        const Ice::CommunicatorHolder ich(argc, argv, "config.locator");
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
            auto adapter = communicator->createObjectAdapter("Locator");
            auto registry = make_shared<LocatorRegistryI>();

            auto registryPrx =
                Ice::uncheckedCast<Ice::LocatorRegistryPrx>(adapter->add(registry, Ice::stringToIdentity("registry")));

            adapter->add(
                make_shared<LocatorI>(std::move(registry), std::move(registryPrx)),
                Ice::stringToIdentity("locator"));
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
