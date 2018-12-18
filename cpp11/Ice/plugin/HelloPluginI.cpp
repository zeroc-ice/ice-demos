// **********************************************************************
//
// Copyright (c) 2003-present ZeroC, Inc. All rights reserved.
//
// **********************************************************************

#include <Ice/Ice.h>
#include <Hello.h>

using namespace std;

namespace
{

class HelloI : public Demo::Hello
{
public:

    virtual void
    sayHello(const Ice::Current& current) override
    {
        current.adapter->getCommunicator()->getLogger()->print("Hello World!");
    }

    virtual void
    shutdown(const Ice::Current& current) override
    {
        current.adapter->getCommunicator()->getLogger()->print("Shutting down...");
        current.adapter->getCommunicator()->shutdown();
    }
};

class HelloPluginI : public Ice::Plugin
{
public:

    HelloPluginI(const shared_ptr<Ice::Communicator>& communicator) :
        _communicator(communicator)
    {
    }

    void
    initialize() override
    {
        auto adapter = _communicator->createObjectAdapter("Hello");
        adapter->add(make_shared<HelloI>(), Ice::stringToIdentity("hello"));
        adapter->activate();
    }

    void
    destroy() override
    {
    }

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
