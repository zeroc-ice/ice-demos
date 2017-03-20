// **********************************************************************
//
// Copyright (c) 2003-2017 ZeroC, Inc. All rights reserved.
//
// **********************************************************************

#include <SessionFactoryI.h>
#include <SessionI.h>

using namespace std;
using namespace Demo;

namespace
{

class ClosedCallbackI : public Ice::ConnectionCallback
{
public:

    ClosedCallbackI(const SessionFactoryIPtr& sessionFactory) :
        _sessionFactory(sessionFactory)
    {
    }

    virtual void heartbeat(const Ice::ConnectionPtr&)
    {
        // Ignored
    }

    //
    // The client died or otherwise closed its connection
    // to this server, so we cleanup the associated session
    //
    virtual void closed(const Ice::ConnectionPtr& con)
    {
        _sessionFactory->deadClient(con);
    }

private:

    SessionFactoryIPtr _sessionFactory;
};

}

SessionPrx
SessionFactoryI::create(const string& name, const Ice::Current& current)
{
    SessionIPtr session = new SessionI(name);
    SessionPrx proxy = SessionPrx::uncheckedCast(current.adapter->addWithUUID(session));

    {
        IceUtil::Mutex::Lock sync(_mutex);

        //
        // With this demo, the connection cannot have an old session associated with it
        //
        assert(!_connectionMap[current.con]);

        _connectionMap[current.con] = proxy;
    }

    //
    // Never close this connection from the client and turn on heartbeats with a timeout of 30s
    //
    current.con->setACM(30, Ice::CloseOff, Ice::HeartbeatAlways);
    current.con->setCallback(new ClosedCallbackI(this));

    return proxy;
}

void
SessionFactoryI::shutdown(const Ice::Current& current)
{
    cout << "Shutting down..." << endl;
    current.adapter->getCommunicator()->shutdown();
}

void
SessionFactoryI::deadClient(const Ice::ConnectionPtr& con)
{
    SessionPrx session;
    {
        IceUtil::Mutex::Lock sync(_mutex);
        std::map<Ice::ConnectionPtr, Demo::SessionPrx>::iterator p = _connectionMap.find(con);
        if(p != _connectionMap.end())
        {
            session = p->second;
            _connectionMap.erase(p);
        }
    }

    if(session)
    {
        try
        {
            session->destroy();
            cout << "Cleaned up dead client." << endl;
        }
        catch(const Ice::ObjectNotExistException&)
        {
            // The client already destroyed this session
        }
        catch(const Ice::ConnectionRefusedException&)
        {
            // Server is shutting down
        }
    }
}
