// **********************************************************************
//
// Copyright (c) 2003-2016 ZeroC, Inc. All rights reserved.
//
// **********************************************************************

import Demo.*;

class Glacier2SessionManagerI implements com.zeroc.Glacier2.SessionManager
{
    @Override
    public com.zeroc.Glacier2.SessionPrx create(String userId, com.zeroc.Glacier2.SessionControlPrx control,
                                                com.zeroc.Ice.Current c)
    {
        SessionI session = new SessionI(_logger, c.adapter);

        com.zeroc.Glacier2.SessionPrx proxy =
            com.zeroc.Glacier2.SessionPrx.uncheckedCast(c.adapter.addWithUUID(session));

        _logger.trace("SessionFactory", "create new session: " +
                      com.zeroc.Ice.Util.identityToString(proxy.ice_getIdentity()));

        _reaper.add(proxy, session);

        return proxy;
    }

    Glacier2SessionManagerI(com.zeroc.Ice.Logger logger, ReapTask reaper)
    {
        _logger = logger;
        _reaper = reaper;
    }

    private com.zeroc.Ice.Logger _logger;
    private ReapTask _reaper;
}
