// **********************************************************************
//
// Copyright (c) 2003-present ZeroC, Inc. All rights reserved.
//
// **********************************************************************

#include <SessionFactoryI.h>
#include <SessionI.h>

using namespace std;
using namespace Demo;

namespace
{

class CloseCallbackI : public Ice::CloseCallback
{
public:

    CloseCallbackI(const SessionPrx& session) :
        _session(session)
    {
    }

    //
    // The client died or otherwise closed its connection to this server, so we
    // cleanup the associated session
    //
    virtual void closed(const Ice::ConnectionPtr&)
    {
        try
        {
            _session->destroy();
            cout << "Cleaned up dead client." << endl;
        }
        catch(const Ice::LocalException&)
        {
            // The client already destroyed this session, or the server is shutting down
        }
    }

private:

    SessionPrx _session;
};

}

SessionPrx
SessionFactoryI::create(const string& name, const Ice::Current& current)
{
    SessionIPtr session = new SessionI(name);
    SessionPrx proxy = SessionPrx::uncheckedCast(current.adapter->addWithUUID(session));

    //
    // Remove endpoints to ensure that calls are collocated-only
    // This way, if we invoke on the proxy during shutdown, the invocation fails immediately
    // without attempting to establish any connection
    //
    SessionPrx collocProxy = proxy->ice_endpoints(Ice::EndpointSeq());

    //
    // Never close this connection from the client and turn on heartbeats with a timeout of 30s
    //
    current.con->setACM(30, Ice::CloseOff, Ice::HeartbeatAlways);
    current.con->setCloseCallback(new CloseCallbackI(collocProxy));

    return proxy;
}

void
SessionFactoryI::shutdown(const Ice::Current& current)
{
    cout << "Shutting down..." << endl;
    current.adapter->getCommunicator()->shutdown();
}
