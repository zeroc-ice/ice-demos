// **********************************************************************
//
// Copyright (c) 2003-present ZeroC, Inc. All rights reserved.
//
// **********************************************************************

using Demo;
using System;
using System.Collections.Generic;

public class SessionI : SessionDisp_
{
    public SessionI(string name)
    {
        _name = name;
        _nextId = 0;
        _destroy = false;
        _objs = new List<HelloPrx>();

        Console.Out.WriteLine("The session " + _name + " is now created.");
    }

    public override HelloPrx createHello(Ice.Current current)
    {
        lock(this)
        {
            if(_destroy)
            {
                throw new Ice.ObjectNotExistException();
            }

            var hello = HelloPrxHelper.uncheckedCast(current.adapter.addWithUUID(new HelloI(_name, _nextId++)));
            _objs.Add(hello);
            return hello;
        }
    }

    public override string getName(Ice.Current c)
    {
        lock(this)
        {
            if(_destroy)
            {
                throw new Ice.ObjectNotExistException();
            }

            return _name;
        }
    }

    public override void destroy(Ice.Current current)
    {
        lock(this)
        {
            if(_destroy)
            {
                throw new Ice.ObjectNotExistException();
            }

            _destroy = true;

            Console.Out.WriteLine("The session " + _name +  " is now destroyed.");
            try
            {
                current.adapter.remove(current.id);
                foreach(HelloPrx p in _objs)
                {
                    current.adapter.remove(p.ice_getIdentity());
                }
            }
            catch(Ice.ObjectAdapterDeactivatedException)
            {
                // This method is called on shutdown of the server, in which
                // case this exception is expected.
            }
        }

        _objs.Clear();
    }

    private string _name;
    private int _nextId; // The per-session id of the next hello object. This is used for tracing purposes.
    private List<HelloPrx> _objs; // List of per-session allocated hello objects.
    private bool _destroy;
}
