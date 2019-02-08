//
// Copyright (c) ZeroC, Inc. All rights reserved.
//

import com.zeroc.demos.Database.library.Demo.*;

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
        _manager.destroy(c);
        c.adapter.remove(_library.ice_getIdentity());
        c.adapter.remove(c.id);
    }

    SessionI(com.zeroc.Ice.Logger logger, com.zeroc.Ice.ObjectAdapter adapter, SessionManager manager)
    {
        _logger = logger;
        _libraryI = new LibraryI();
        _library = LibraryPrx.uncheckedCast(adapter.addWithUUID(new DispatchInterceptorI(_libraryI)));
        _manager = manager;
    }

    private com.zeroc.Ice.Logger _logger;
    private boolean _destroyed = false; // true if destroy() was called, false otherwise.
    private LibraryPrx _library;
    private LibraryI _libraryI;
    private SessionManager _manager;
}
