// **********************************************************************
//
// Copyright (c) 2003-present ZeroC, Inc. All rights reserved.
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

    public override void addClient(CallbackReceiverPrx client, Ice.Current current)
    {
        lock(this)
        {
            Console.Out.WriteLine("adding client `" + Ice.Util.identityToString(client.ice_getIdentity()) + "'");
            _clients.Add((CallbackReceiverPrx)client.ice_fixed(current.con));
        }
    }

    public void Run()
    {
        int num = 0;
        lock(this)
        {
            while(!_destroy)
            {
                System.Threading.Monitor.Wait(this, 2000);

                if(!_destroy && _clients.Count > 0)
                {
                    ++num;

                    //
                    // Invoke callback on all clients; it's safe to do it with _mutex locked
                    // because Ice guarantees these async invocations never block the calling thread.
                    //
                    // The exception callback, if called, is called by an Ice thread, and never the
                    // calling thread.
                    foreach(var c in _clients)
                    {
                        c.callbackAsync(num).ContinueWith(t => {
                                if(t.Exception != null)
                                {
                                    removeClient(c, t.Exception.InnerException);
                                }
                            });
                    }
                }
            }
        }
    }

    private void removeClient(CallbackReceiverPrx c, Exception ex)
    {
        Console.Error.WriteLine("removing client `" + Ice.Util.identityToString(c.ice_getIdentity()) + "':\n" + ex);
        lock(this)
        {
            _clients.Remove(c);
        }
    }

    private Ice.Communicator _communicator;
    private bool _destroy;
    private List<CallbackReceiverPrx> _clients;
}
