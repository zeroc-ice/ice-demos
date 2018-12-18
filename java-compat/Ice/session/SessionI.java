// **********************************************************************
//
// Copyright (c) 2003-present ZeroC, Inc. All rights reserved.
//
// **********************************************************************

import Demo.*;

class SessionI extends _SessionDisp
{
    public SessionI(String name)
    {
        _name = name;
        System.out.println("The session " + _name + " is now created.");
    }

    @Override
    synchronized public HelloPrx createHello(Ice.Current c)
    {
        if(_destroy)
        {
            throw new Ice.ObjectNotExistException();
        }
        HelloPrx hello = HelloPrxHelper.uncheckedCast(c.adapter.addWithUUID(new HelloI(_name, _nextId++)));
        _objs.add(hello);
        return hello;
    }

    @Override
    synchronized public String getName(Ice.Current c)
    {
        if(_destroy)
        {
            throw new Ice.ObjectNotExistException();
        }
        return _name;
    }

    @Override
    synchronized public void destroy(Ice.Current c)
    {
        if(_destroy)
        {
            throw new Ice.ObjectNotExistException();
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
        catch(Ice.ObjectAdapterDeactivatedException e)
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
