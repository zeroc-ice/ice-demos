//
// Copyright (c) ZeroC, Inc. All rights reserved.
//

package com.zeroc.library.controller;

import android.os.Handler;

public class SessionController
{
    public interface Listener
    {
        void onDestroy();
    }

    private Handler _handler;
    private com.zeroc.Ice.Communicator _communicator;
    private QueryController _queryController;
    private Listener _sessionListener;
    private boolean _fatal = false;
    private boolean _destroyed = false;

    String _username;

    private SessionAdapter _session;

    synchronized private void postSessionDestroyed()
    {
        _fatal = true;
        if(_sessionListener != null)
        {
            final Listener listener = _sessionListener;
            _handler.post(new Runnable()
            {
                public void run()
                {
                    listener.onDestroy();
                }
            });
        }
    }

    SessionController(Handler handler, com.zeroc.Ice.Communicator communicator, SessionAdapter session, String username)
    {
        _communicator = communicator;
        _session = session;
        _handler = handler;

        _username = username;

        _queryController = new QueryController(_handler, _session.getLibrary());
    }

    synchronized public void destroy()
    {
        if(_destroyed)
        {
            return;
        }
        _destroyed = true;
        _username = null;

        new Thread(() ->
            {
                _queryController.destroy();

                try
                {
                    _session.destroy();
                }
                catch(Exception e)
                {
                }

                _communicator.destroy();
            }).start();
    }

    synchronized public String getUsername()
    {
        return _username;
    }

    synchronized public void setSessionListener(Listener listener)
    {
        _sessionListener = listener;
        if(_fatal)
        {
            listener.onDestroy();
        }
    }

    synchronized public QueryController createQuery(QueryController.Listener listener, QueryController.QueryType t,
                                                    String q)
    {
        _queryController.destroy();
        _queryController = new QueryController(_handler, _session.getLibrary(), listener, t, q);
        return _queryController;
    }

    synchronized public QueryController getCurrentQuery()
    {
        return _queryController;
    }
}
