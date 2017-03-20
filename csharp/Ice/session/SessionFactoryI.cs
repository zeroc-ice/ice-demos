// **********************************************************************
//
// Copyright (c) 2003-2017 ZeroC, Inc. All rights reserved.
//
// **********************************************************************

using Demo;
using System;
using System.Collections.Generic;
using System.Diagnostics;

public class SessionFactoryI : SessionFactoryDisp_
{
    public override SessionPrx create(string name, Ice.Current current)
    {
        var session = new SessionI(name);
        var proxy = SessionPrxHelper.uncheckedCast(current.adapter.addWithUUID(session));

        lock(this)
        {
            //
            // With this demo, the connection cannot have an old session associated with it
            //
            Debug.Assert(!_connectionMap.ContainsKey(current.con));
            _connectionMap[current.con] = proxy;
        }

        //
        // Never close this connection from the client and turn on heartbeats with a timeout of 30s
        //
        current.con.setACM(30, Ice.ACMClose.CloseOff, Ice.ACMHeartbeat.HeartbeatAlways);
        current.con.setCallback(new ClosedCallbackI(this));

        return proxy;
    }

    public override void shutdown(Ice.Current current)
    {
        Console.Out.WriteLine("Shutting down...");
        current.adapter.getCommunicator().shutdown();
    }

    void deadClient(Ice.Connection con)
    {
        SessionPrx session = null;
        lock(this)
        {
            if(_connectionMap.ContainsKey(con))
            {
                session = _connectionMap[con];
                _connectionMap.Remove(con);
            }
        }

        if(session != null)
        {
            try
            {
                session.destroy();
                Console.Out.WriteLine("Cleaned up dead client.");
            }
            catch(Ice.ObjectNotExistException)
            {
                // The client already destroyed this session
            }
            catch(Ice.ConnectionRefusedException)
            {
                // Server is shutting down
            }
        }
    }

    class ClosedCallbackI : Ice.ConnectionCallback
    {
        internal ClosedCallbackI(SessionFactoryI sessionFactory)
        {
            _sessionFactory = sessionFactory;
        }

        public void heartbeat(Ice.Connection con)
        {
            // ignored
        }

        public void closed(Ice.Connection con)
        {
            _sessionFactory.deadClient(con);
        }

        private SessionFactoryI _sessionFactory;
    }

    private Dictionary<Ice.Connection, SessionPrx> _connectionMap = new Dictionary<Ice.Connection, SessionPrx>();
}
