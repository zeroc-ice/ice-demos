//
// Copyright (c) ZeroC, Inc. All rights reserved.
//

import com.zeroc.demos.Ice.session.Demo.*;

class SessionI implements Session
{
    public SessionI(String name)
    {
        _name = name;
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

    private String _name;
    private boolean _destroy = false; // true if destroy() was called, false otherwise.
    private int _nextId = 0; // The id of the next hello object. This is used for tracing purposes.
    private java.util.List<HelloPrx> _objs =
        new java.util.LinkedList<>(); // List of per-client allocated Hello objects.
}
