// **********************************************************************
//
// Copyright (c) 2003-2016 ZeroC, Inc. All rights reserved.
//
// **********************************************************************

import Demo.*;

class SessionI implements Session
{
    public SessionI(String name)
    {
        _name = name;
        _timestamp = System.currentTimeMillis();
        System.out.println("The session " + _name + " is now created.");
    }

    @Override
    synchronized public HelloPrx createHello(com.zeroc.Ice.Current c)
    {
        if(_destroy)
        {
            throw new com.zeroc.Ice.ObjectNotExistException();
        }
        HelloPrx hello = HelloPrx.uncheckedCast(c.adapter.addWithUUID(new HelloI(_name, _nextId++)));
        _objs.add(hello);
        return hello;
    }

    @Override
    synchronized public void refresh(com.zeroc.Ice.Current c)
    {
        if(_destroy)
        {
            throw new com.zeroc.Ice.ObjectNotExistException();
        }
        _timestamp = System.currentTimeMillis();
    }

    @Override
    synchronized public String getName(com.zeroc.Ice.Current c)
    {
        if(_destroy)
        {
            throw new com.zeroc.Ice.ObjectNotExistException();
        }
        return _name;
    }
    
    @Override
    synchronized public void destroy(com.zeroc.Ice.Current c)
    {
        if(_destroy)
        {
            throw new com.zeroc.Ice.ObjectNotExistException();
        }

        _destroy = true;
        System.out.println("The session " + _name + " is now destroyed.");
        try
        {
            c.adapter.remove(c.id);
            for(HelloPrx p : _objs)
            {
                c.adapter.remove(p.ice_getIdentity());
            }
        }
        catch(com.zeroc.Ice.ObjectAdapterDeactivatedException e)
        {
            // This method is called on shutdown of the server, in
            // which case this exception is expected.
        }
        _objs.clear();
    }

    synchronized public long timestamp()
    {
        if(_destroy)
        {
            throw new com.zeroc.Ice.ObjectNotExistException();
        }
        return _timestamp;
    }

    private String _name;
    private boolean _destroy = false; // true if destroy() was called, false otherwise.
    private long _timestamp; // The last time the session was refreshed.
    private int _nextId = 0; // The id of the next hello object. This is used for tracing purposes.
    private java.util.List<HelloPrx> _objs =
        new java.util.LinkedList<>(); // List of per-client allocated Hello objects.
}
