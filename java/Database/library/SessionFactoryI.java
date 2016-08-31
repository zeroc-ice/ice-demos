// **********************************************************************
//
// Copyright (c) 2003-2016 ZeroC, Inc. All rights reserved.
//
// **********************************************************************

import Demo.*;

class SessionFactoryI implements SessionFactory
{
    @Override
    public synchronized SessionPrx create(com.zeroc.Ice.Current c)
    {
        SessionI session = new SessionI(_logger, c.adapter);

        SessionPrx proxy = SessionPrx.uncheckedCast(c.adapter.addWithUUID(session));

        _logger.trace("SessionFactory", "create new session: " +
                      com.zeroc.Ice.Util.identityToString(proxy.ice_getIdentity()));

        _reaper.add(proxy, session);

        return proxy;
    }

    @Override
    public long getSessionTimeout(com.zeroc.Ice.Current c)
    {
        return _timeout;
    }

    SessionFactoryI(com.zeroc.Ice.Logger logger, ReapTask reaper, long timeout)
    {
        _logger = logger;
        _reaper = reaper;
        _timeout = timeout;
    }

    private com.zeroc.Ice.Logger _logger;
    private ReapTask _reaper;
    private long _timeout;
}
