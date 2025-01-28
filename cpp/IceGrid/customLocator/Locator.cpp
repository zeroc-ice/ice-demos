// Copyright (c) ZeroC, Inc.

#include <Ice/Ice.h>
#include <iostream>

using namespace std;

namespace
{

    //
    // This LocatorRegistryI forwards everything to the provided (IceGrid) registry
    //

    class LocatorRegistryI : public Ice::LocatorRegistry
    {
    public:
        explicit LocatorRegistryI(const optional<Ice::LocatorRegistryPrx>& registry) : _registry(registry) {}

        void setAdapterDirectProxyAsync(
            string id,
            optional<Ice::ObjectPrx> proxy,
            function<void()> response,
            function<void(exception_ptr)> exception,
            const Ice::Current&) final
        {
            // AMI call using the AMD callbacks
            _registry->setAdapterDirectProxyAsync(id, proxy, response, exception);
        }

        void setReplicatedAdapterDirectProxyAsync(
            string id,
            string group,
            optional<Ice::ObjectPrx> proxy,
            function<void()> response,
            function<void(exception_ptr)> exception,
            const Ice::Current&) final
        {
            _registry->setReplicatedAdapterDirectProxyAsync(id, group, proxy, response, exception);
        }

        void setServerProcessProxyAsync(
            string id,
            optional<Ice::ProcessPrx> proxy,
            function<void()> response,
            function<void(exception_ptr)> exception,
            const Ice::Current&) final
        {
            _registry->setServerProcessProxyAsync(id, proxy, response, exception);
        }

    private:
        const optional<Ice::LocatorRegistryPrx> _registry;
    };

    //
    // Likewise, LocatorI forwards everything to the provided (IceGrid) locator
    // when the context contains SECRET == LetMeIn
    //
    class LocatorI : public Ice::Locator
    {
    public:
        LocatorI(const optional<Ice::LocatorPrx>& locator, const optional<Ice::LocatorRegistryPrx>& registry)
            : _locator(locator),
              _registry(registry)
        {
        }

        void findObjectByIdAsync(
            Ice::Identity id,
            function<void(const optional<Ice::ObjectPrx>&)> response,
            function<void(exception_ptr)> exception,
            const Ice::Current& current) const final
        {
            auto p = current.ctx.find("SECRET");
            if (p != current.ctx.end() && p->second == "LetMeIn")
            {
                _locator->findObjectByIdAsync(id, response, exception);
            }
            else
            {
                response(nullopt);
            }
        }

        void findAdapterByIdAsync(
            string id,
            function<void(const optional<Ice::ObjectPrx>&)> response,
            function<void(exception_ptr)> exception,
            const Ice::Current& current) const final
        {
            auto p = current.ctx.find("SECRET");
            if (p != current.ctx.end() && p->second == "LetMeIn")
            {
                _locator->findAdapterByIdAsync(id, response, exception);
            }
            else
            {
                response(nullopt);
            }
        }

        [[nodiscard]]
        optional<Ice::LocatorRegistryPrx> getRegistry(const Ice::Current&) const final
        {
            return _registry;
        }

    private:
        const optional<Ice::LocatorPrx> _locator;
        const optional<Ice::LocatorRegistryPrx> _registry;
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
            //
            // Get the IceGrid Locator and LocatorRegistry
            //
            auto defaultLocator = communicator->getDefaultLocator();
            auto defaultRegistry = defaultLocator->getRegistry();

            //
            // Create the custom Locator and LocatorRegistry
            //
            auto adapter = communicator->createObjectAdapter("Locator");
            auto registry = make_shared<LocatorRegistryI>(defaultRegistry);
            auto registryPrx =
                Ice::uncheckedCast<Ice::LocatorRegistryPrx>(adapter->add(registry, Ice::stringToIdentity("registry")));
            auto locator = make_shared<LocatorI>(defaultLocator, registryPrx);
            adapter->add(locator, Ice::stringToIdentity("locator"));
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
