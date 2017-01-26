// **********************************************************************
//
// Copyright (c) 2003-2016 ZeroC, Inc. All rights reserved.
//
// **********************************************************************

import Demo.*;

class SessionFactoryI extends _SessionFactoryDisp
{
    SessionFactoryI(ReapTask reaper)
    {
        _reaper = reaper;
    }

    @Override
    public synchronized SessionPrx
    create(String name, Ice.Current c)
    {
        SessionI session = new SessionI(name);
        SessionPrx proxy = SessionPrxHelper.uncheckedCast(c.adapter.addWithUUID(session));
        _reaper.add(proxy, session);
        return proxy;
    }

    @Override
    public void
    shutdown(Ice.Current c)
    {
        System.out.println("Shutting down...");
        c.adapter.getCommunicator().shutdown();
    }

    private ReapTask _reaper;
}
