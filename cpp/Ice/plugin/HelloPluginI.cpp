// Copyright (c) ZeroC, Inc.

#include <Hello.h>
#include <Ice/Ice.h>

using namespace std;

namespace
{

    class HelloI final : public Demo::Hello
    {
    public:
        void sayHello(const Ice::Current& current) final
        {
            current.adapter->getCommunicator()->getLogger()->print("Hello World!");
        }

        void shutdown(const Ice::Current& current) final
        {
            current.adapter->getCommunicator()->getLogger()->print("Shutting down...");
            current.adapter->getCommunicator()->shutdown();
        }
    };

    class HelloPluginI final : public Ice::Plugin
    {
    public:
        HelloPluginI(const shared_ptr<Ice::Communicator>& communicator) : _communicator(communicator) {}

        void initialize() final
        {
            auto adapter = _communicator->createObjectAdapter("Hello");
            adapter->add(make_shared<HelloI>(), Ice::stringToIdentity("hello"));
            adapter->activate();
        }

        void destroy() final {}

    private:
        shared_ptr<Ice::Communicator> _communicator;
    };

};

extern "C"
{
    ICE_DECLSPEC_EXPORT ::Ice::Plugin*
    createHello(const shared_ptr<Ice::Communicator>& communicator, const string&, const Ice::StringSeq&)
    {
        return new HelloPluginI(communicator);
    }
}
