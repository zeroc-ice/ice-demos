// Copyright (c) ZeroC, Inc.

#include "HelloServiceI.h"
#include "HelloI.h"

#include <Ice/Ice.h>
#include <iostream>

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
    _adapter = communicator->createObjectAdapterWithEndpoints("Hello", "tcp -p 10000:udp -p 10000");
    _adapter->add(make_shared<HelloI>(), Ice::stringToIdentity("hello"));
    _adapter->activate();
    cout << "Listening on port 10000..." << endl;
}

void
HelloServiceI::stop()
{
    _adapter->destroy();
}
