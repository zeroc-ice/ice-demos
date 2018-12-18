// **********************************************************************
//
// Copyright (c) 2003-present ZeroC, Inc. All rights reserved.
//
// **********************************************************************

import Demo.*;

class SessionFactoryI extends _SessionFactoryDisp
{
    @Override
    public synchronized SessionPrx create(String name, Ice.Current current)
    {
        SessionI session = new SessionI(name);
        SessionPrx proxy = SessionPrxHelper.uncheckedCast(current.adapter.addWithUUID(session));

        //
        // Remove endpoints to ensure that calls are collocated-only
        // This way, if we invoke on the proxy during shutdown, the invocation fails immediately
        // without attempting to establish any connection
        //
        final SessionPrx collocProxy = SessionPrxHelper.uncheckedCast(proxy.ice_endpoints(new Ice.Endpoint[0]));

        //
        // Never close this connection from the client and turn on heartbeats with a timeout of 30s
        //
        current.con.setACM(Ice.Optional.O(30),
                           Ice.Optional.O(Ice.ACMClose.CloseOff),
                           Ice.Optional.O(Ice.ACMHeartbeat.HeartbeatAlways));
        current.con.setCloseCallback(new Ice.CloseCallback()
                {
                    @Override
                    public void closed(Ice.Connection con)
                    {
                        try
                        {
                            collocProxy.destroy();
                            System.out.println("Cleaned up dead client.");
                        }
                        catch(Ice.LocalException ex)
                        {
                            // The client already destroyed this session, or the server is shutting down
                        }
                    }
                });
        return proxy;
    }

    @Override
    public void shutdown(Ice.Current current)
    {
        System.out.println("Shutting down...");
        current.adapter.getCommunicator().shutdown();
    }
}
