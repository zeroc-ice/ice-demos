// **********************************************************************
//
// Copyright (c) 2003-2016 ZeroC, Inc. All rights reserved.
//
// **********************************************************************

import Demo.*;

class SessionI implements Glacier2Session
{
    @Override
    synchronized public LibraryPrx getLibrary(com.zeroc.Ice.Current c)
    {
        if(_destroyed)
        {
            throw new com.zeroc.Ice.ObjectNotExistException();
        }
        return _library;
    }

    @Override
    synchronized public void refresh(com.zeroc.Ice.Current c)
    {
        if(_destroyed)
        {
            throw new com.zeroc.Ice.ObjectNotExistException();
        }
        _timestamp = System.currentTimeMillis();
    }

    @Override
    synchronized public void destroy(com.zeroc.Ice.Current c)
    {
        if(_destroyed)
        {
            throw new com.zeroc.Ice.ObjectNotExistException();
        }

        _destroyed = true;
        _logger.trace("Session", "session " + com.zeroc.Ice.Util.identityToString(c.id) + " is now destroyed.");

        // This method is never called on shutdown of the server.
        _libraryI.destroy();
        c.adapter.remove(_library.ice_getIdentity());
        c.adapter.remove(c.id);
    }

    // Called on application shutdown.
    synchronized public void shutdown()
    {
        if(!_destroyed)
        {
            _destroyed = true;
            _libraryI.shutdown();
        }
    }

    synchronized public long timestamp()
    {
        if(_destroyed)
        {
            throw new com.zeroc.Ice.ObjectNotExistException();
        }
        return _timestamp;
    }

    SessionI(com.zeroc.Ice.Logger logger, com.zeroc.Ice.ObjectAdapter adapter)
    {
        _logger = logger;
        _timestamp = System.currentTimeMillis();
        _libraryI = new LibraryI();
        _library = LibraryPrx.uncheckedCast(adapter.addWithUUID(new DispatchInterceptorI(_libraryI)));
    }

    private com.zeroc.Ice.Logger _logger;
    private boolean _destroyed = false; // true if destroy() was called, false otherwise.
    private long _timestamp; // The last time the session was refreshed.
    private LibraryPrx _library;
    private LibraryI _libraryI;
}
