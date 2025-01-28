// Copyright (c) ZeroC, Inc.

#include "SessionFactoryI.h"
#include "SessionI.h"
#include <iostream>

using namespace std;
using namespace Demo;

optional<SessionPrx>
SessionFactoryI::create(string name, const Ice::Current& current)
{
    auto session = make_shared<SessionI>(name);
    auto proxy = Ice::uncheckedCast<SessionPrx>(current.adapter->addWithUUID(session));

    //
    // Remove endpoints to ensure that calls are collocated-only
    // This way, if we invoke on the proxy during shutdown, the invocation fails immediately
    // without attempting to establish any connection
    //
    auto collocProxy = proxy->ice_endpoints(Ice::EndpointSeq());

    current.con->setCloseCallback(
        [collocProxy](const shared_ptr<Ice::Connection>&)
        {
            try
            {
                collocProxy->destroy();
                cout << "Cleaned up dead client." << endl;
            }
            catch (const Ice::LocalException&)
            {
                // The client already destroyed this session, or the server is shutting down
            }
        });

    return proxy;
}

void
SessionFactoryI::shutdown(const Ice::Current& current)
{
    cout << "Shutting down..." << endl;
    current.adapter->getCommunicator()->shutdown();
}
