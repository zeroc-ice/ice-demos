//
// Copyright (c) ZeroC, Inc. All rights reserved.
//

import com.zeroc.demos.Database.library.Demo.*;

import com.zeroc.Ice.ACMClose;
import com.zeroc.Ice.ACMHeartbeat;

class SessionFactoryI implements SessionFactory, SessionManager
{
    @Override
    public synchronized SessionPrx create(com.zeroc.Ice.Current current)
    {
        SessionI session = new SessionI(_logger, current.adapter, this);

        SessionPrx proxy = SessionPrx.uncheckedCast(current.adapter.addWithUUID(session));

        _logger.trace("SessionFactory", "create new session: " +
                      com.zeroc.Ice.Util.identityToString(proxy.ice_getIdentity()));

        //
        // Remove endpoints to ensure that calls are collocated-only
        // This way, if we invoke on the proxy during shutdown, the invocation fails immediately
        // without attempting to establish any connection
        //
        final SessionPrx collocProxy = proxy.ice_endpoints(new com.zeroc.Ice.Endpoint[0]);

        //
        // Never close this connection from the client and turn on heartbeats
        //
        current.con.setACM(java.util.OptionalInt.of(30),
                           java.util.Optional.of(ACMClose.CloseOff),
                           java.util.Optional.of(ACMHeartbeat.HeartbeatAlways));

        current.con.setCloseCallback((con) ->
                                    {
                                        try
                                        {
                                            collocProxy.destroy();
                                            _logger.trace("SessionFactory", "Cleaned up dead client.");
                                        }
                                        catch(com.zeroc.Ice.LocalException ex)
                                        {
                                            // The client already destroyed this session, or the server is shutting down
                                        }
                                    });

        return proxy;
    }

    @Override
    public void destroy(com.zeroc.Ice.Current current)
    {
    }

    SessionFactoryI(com.zeroc.Ice.Logger logger)
    {
        _logger = logger;
    }

    final private com.zeroc.Ice.Logger _logger;
}
