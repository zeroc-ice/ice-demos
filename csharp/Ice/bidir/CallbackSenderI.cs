// **********************************************************************
//
// Copyright (c) 2003-2016 ZeroC, Inc. All rights reserved.
//
// **********************************************************************

using Demo;
using System;
using System.Collections.Generic;

class CallbackSenderI : CallbackSenderDisp_
{
    public CallbackSenderI(Ice.Communicator communicator)
    {
        _communicator = communicator;
        _destroy = false;
        _clients = new List<CallbackReceiverPrx>();
    }

    public void destroy()
    {
        lock(this)
        {
            Console.Out.WriteLine("destroying callback sender");
            _destroy = true;
            System.Threading.Monitor.Pulse(this);
        }
    }

    public override void addClient(Ice.Identity ident, Ice.Current current)
    {
        lock(this)
        {
            Console.Out.WriteLine("adding client `" + Ice.Util.identityToString(ident) + "'");
            _clients.Add(CallbackReceiverPrxHelper.uncheckedCast(current.con.createProxy(ident)));
        }
    }

    public void Run()
    {
        int num = 0;
        while(true)
        {
            List<CallbackReceiverPrx> clients;
            lock(this)
            {
                System.Threading.Monitor.Wait(this, 2000);
                if(_destroy)
                {
                    break;
                }

                clients = new List<CallbackReceiverPrx>(_clients);
            }

            if(clients.Count > 0)
            {
                ++num;
                foreach(var c in clients)
                {
                    try
                    {
                        c.callback(num);
                    }
                    catch(Ice.LocalException ex)
                    {
                        Console.Error.WriteLine("removing client `" +
                                                Ice.Util.identityToString(c.ice_getIdentity()) + "':\n" + ex);

                        lock(this)
                        {
                            _clients.Remove(c);
                        }
                    }
                }
            }
        }
    }

    private Ice.Communicator _communicator;
    private bool _destroy;
    private List<CallbackReceiverPrx> _clients;
}
