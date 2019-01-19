//
// Copyright (c) ZeroC, Inc. All rights reserved.
//

import com.zeroc.demos.Ice.session.Demo.*;

class SessionFactoryI implements SessionFactory
{
    @Override
    public SessionPrx create(String name, com.zeroc.Ice.Current current)
    {
        SessionI session = new SessionI(name);
        SessionPrx proxy = SessionPrx.uncheckedCast(current.adapter.addWithUUID(session));

        //
        // Remove endpoints to ensure that calls are collocated-only
        // This way, if we invoke on the proxy during shutdown, the invocation fails immediately
        // without attempting to establish any connection
        //
        SessionPrx collocProxy = proxy.ice_endpoints(new com.zeroc.Ice.Endpoint[0]);

        //
        // Never close this connection from the client and turn on heartbeats with a timeout of 30s
        //
        current.con.setACM(java.util.OptionalInt.of(30),
                           java.util.Optional.of(com.zeroc.Ice.ACMClose.CloseOff),
                           java.util.Optional.of(com.zeroc.Ice.ACMHeartbeat.HeartbeatAlways));

        current.con.setCloseCallback(con ->
                 {
                     try
                     {
                         collocProxy.destroy();
                         System.out.println("Cleaned up dead client.");
                     }
                     catch(com.zeroc.Ice.LocalException ex)
                     {
                         // The client already destroyed this session, or the server is shutting down
                     }
                 });

        return proxy;
    }

    @Override
    public void shutdown(com.zeroc.Ice.Current current)
    {
        System.out.println("Shutting down...");
        current.adapter.getCommunicator().shutdown();
    }
}
