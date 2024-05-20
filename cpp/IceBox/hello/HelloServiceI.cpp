//
// Copyright (c) ZeroC, Inc. All rights reserved.
//

#include <HelloI.h>
#include <HelloServiceI.h>
#include <Ice/Ice.h>

using namespace std;

extern "C"
{
    //
    // Factory function
    //
    ICE_DECLSPEC_EXPORT IceBox::Service* create(const shared_ptr<Ice::Communicator>&) { return new HelloServiceI; }
}

void
HelloServiceI::start(
    const string& name,
    const shared_ptr<Ice::Communicator>& communicator,
    const Ice::StringSeq& /*args*/)
{
    _adapter = communicator->createObjectAdapter(name);
    auto hello = make_shared<HelloI>();
    _adapter->add(hello, Ice::stringToIdentity("hello"));
    _adapter->activate();
}

void
HelloServiceI::stop()
{
    _adapter->destroy();
}
