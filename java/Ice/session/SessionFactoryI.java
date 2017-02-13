// **********************************************************************
//
// Copyright (c) 2003-2017 ZeroC, Inc. All rights reserved.
//
// **********************************************************************

import Demo.*;

class SessionFactoryI implements SessionFactory
{
    SessionFactoryI(ReapTask reaper)
    {
        _reaper = reaper;
    }

    @Override
    public synchronized SessionPrx create(String name, com.zeroc.Ice.Current c)
    {
        SessionI session = new SessionI(name);
        SessionPrx proxy = SessionPrx.uncheckedCast(c.adapter.addWithUUID(session));
        _reaper.add(proxy, session);
        return proxy;
    }

    @Override
    public void shutdown(com.zeroc.Ice.Current c)
    {
        System.out.println("Shutting down...");
        c.adapter.getCommunicator().shutdown();
    }

    private ReapTask _reaper;
}
