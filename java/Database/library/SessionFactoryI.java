// **********************************************************************
//
// Copyright (c) 2003-2016 ZeroC, Inc. All rights reserved.
//
// **********************************************************************

import Demo.*;

class SessionFactoryI extends _SessionFactoryDisp
{
    @Override
    public synchronized SessionPrx
    create(Ice.Current c)
    {
        SessionI session = new SessionI(_logger, c.adapter);
        _SessionTie servant = new _SessionTie(session);

        SessionPrx proxy = SessionPrxHelper.uncheckedCast(c.adapter.addWithUUID(servant));

        _logger.trace("SessionFactory", "create new session: " +
                      Ice.Util.identityToString(proxy.ice_getIdentity()));

        _reaper.add(proxy, session);

        return proxy;
    }

    @Override
    public long
    getSessionTimeout(Ice.Current c)
    {
        return _timeout;
    }

    SessionFactoryI(Ice.Logger logger, ReapTask reaper, long timeout)
    {
        _logger = logger;
        _reaper = reaper;
        _timeout = timeout;
    }

    private Ice.Logger _logger;
    private ReapTask _reaper;
    private long _timeout;
}
