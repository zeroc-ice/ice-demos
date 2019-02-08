//
// Copyright (c) ZeroC, Inc. All rights reserved.
//

import com.zeroc.demos.Database.library.Demo.*;

import com.zeroc.Glacier2.SessionPrx;
import com.zeroc.Glacier2.SessionControlPrx;
import com.zeroc.Ice.ACMClose;
import com.zeroc.Ice.ACMHeartbeat;

class Glacier2SessionManagerI implements com.zeroc.Glacier2.SessionManager, SessionManager
{
    @Override
    public synchronized SessionPrx create(String userId, SessionControlPrx control, com.zeroc.Ice.Current current)
    {
        SessionI session = new SessionI(_logger, current.adapter, this);

        SessionPrx proxy = SessionPrx.uncheckedCast(current.adapter.addWithUUID(session));

        _logger.trace("SessionFactory", "create new session: " +
                      com.zeroc.Ice.Util.identityToString(proxy.ice_getIdentity()));

        java.util.List<SessionPrx> sessions = _connectionMap.get(current.con);
        if(sessions == null)
        {
            _logger.trace("SessionFactory", "enter: create new entry in connection map");

            sessions = new java.util.ArrayList<SessionPrx>();
            sessions.add(proxy.ice_endpoints(new com.zeroc.Ice.Endpoint[0]));
            _connectionMap.put(current.con, sessions);

            //
            // Never close this connection from the client and turn on heartbeats with a timeout of 30s
            //
            current.con.setACM(java.util.OptionalInt.of(30),
                               java.util.Optional.of(ACMClose.CloseOff),
                               java.util.Optional.of(ACMHeartbeat.HeartbeatAlways));
            current.con.setCloseCallback((con) ->
                                        {
                                            try
                                            {
                                                deadRouter(con);
                                            }
                                            catch(com.zeroc.Ice.LocalException ex)
                                            {
                                                // The client already destroyed this session, or the server is shutting down
                                            }
                                        });

        }
        else
        {
            _logger.trace("SessionFactory", "create: add session to existing connection map entry");
            sessions.add(proxy.ice_endpoints(new com.zeroc.Ice.Endpoint[0]));
        }

        return proxy;
    }

    void
    deadRouter(com.zeroc.Ice.Connection con)
    {
        _logger.trace("SessionFactory", "Detected dead router - destroying all associated sessions.");

        java.util.List<SessionPrx> sessions;
        {
            synchronized(this)
            {
                sessions = _connectionMap.remove(con);
            }
        }

        for(SessionPrx s : sessions)
        {
            //
            // Collocated calls to the Session
            //
            s.destroy();
        }
    }

    @Override
    public synchronized void destroy(com.zeroc.Ice.Current current)
    {
        java.util.List<SessionPrx> sessions = _connectionMap.get(current.con);
        if(sessions != null)
        {
            sessions.removeIf(s -> s.ice_getIdentity().equals(current.id));
        }
    }

    Glacier2SessionManagerI(com.zeroc.Ice.Logger logger)
    {
        _logger = logger;
    }

    final private com.zeroc.Ice.Logger _logger;

    //
    // Map of connection from Glacier2 to proxies of Sessions in this server
    //
    java.util.Map<com.zeroc.Ice.Connection, java.util.List<SessionPrx>> _connectionMap =
        new java.util.HashMap<com.zeroc.Ice.Connection, java.util.List<SessionPrx>>();
}
