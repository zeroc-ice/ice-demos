// **********************************************************************
//
// Copyright (c) 2003-2016 ZeroC, Inc. All rights reserved.
//
// **********************************************************************

using System;
using System.Threading;
using System.Collections.Generic;
using Demo;

public class ReapThread
{
    public class SessionProxyPair
    {
        public SessionProxyPair(SessionPrx p, SessionI s)
        {
            proxy = p;
            session = s;
        }

        public SessionPrx proxy;
        public SessionI session;
    }

    public ReapThread()
    {
        _timeout = TimeSpan.FromSeconds(10);
        _terminated = false;
        _sessions = new List<SessionProxyPair>();
    }

    public void run()
    {
        lock(this)
        {
            while(!_terminated)
            {
                Monitor.Wait(this, TimeSpan.FromSeconds(1));
                if(!_terminated)
                {
                    var tmp = new List<SessionProxyPair>();
                    foreach(var p in _sessions)
                    {
                        try
                        {
                            //
                            // Session destruction may take time in a
                            // real-world example. Therefore the current time
                            // is computed for each iteration.
                            //
                            if((DateTime.Now - p.session.timestamp()) > _timeout)
                            {
                                string name = p.proxy.getName();
                                p.proxy.destroy();
                                Console.Out.WriteLine("The session " + name + " has timed out.");
                            }
                            else
                            {
                                tmp.Add(p);
                            }
                        }
                        catch(Ice.ObjectNotExistException)
                        {
                            // Ignore.
                        }
                    }
                    _sessions = tmp;
                }
            }
        }
    }

    public void terminate()
    {
        lock(this)
        {
            _terminated = true;
            Monitor.Pulse(this);
            _sessions.Clear();
        }
    }

    public void add(SessionPrx proxy, SessionI session)
    {
        lock(this)
        {
            _sessions.Add(new SessionProxyPair(proxy, session));
        }
    }

    private bool _terminated;
    private TimeSpan _timeout;
    private List<SessionProxyPair> _sessions;
}
