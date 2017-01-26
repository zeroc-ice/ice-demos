// **********************************************************************
//
// Copyright (c) 2003-2016 ZeroC, Inc. All rights reserved.
//
// **********************************************************************

import Demo.*;

class CallbackSenderI implements CallbackSender, java.lang.Runnable
{
    CallbackSenderI(com.zeroc.Ice.Communicator communicator)
    {
        _communicator = communicator;
    }

    synchronized public void destroy()
    {
        System.out.println("destroying callback sender");
        _destroy = true;

        this.notify();
    }

    @Override
    synchronized public void addClient(com.zeroc.Ice.Identity ident, com.zeroc.Ice.Current current)
    {
        System.out.println("adding client `" + com.zeroc.Ice.Util.identityToString(ident) + "'");

        com.zeroc.Ice.ObjectPrx base = current.con.createProxy(ident);
        CallbackReceiverPrx client = CallbackReceiverPrx.uncheckedCast(base);
        _clients.add(client);
    }

    @Override
    public void run()
    {
        int num = 0;
        while(true)
        {
            java.util.List<CallbackReceiverPrx> clients;
            synchronized(this)
            {
                try
                {
                    this.wait(2000);
                }
                catch(java.lang.InterruptedException ex)
                {
                }

                if(_destroy)
                {
                    break;
                }

                clients = new java.util.ArrayList<>(_clients);
            }

            if(!clients.isEmpty())
            {
                ++num;

                for(CallbackReceiverPrx p : clients)
                {
                    try
                    {
                        p.callback(num);
                    }
                    catch(Exception ex)
                    {
                        System.out.println("removing client `" +
                                           com.zeroc.Ice.Util.identityToString(p.ice_getIdentity()) +
                                           "':");
                        ex.printStackTrace();

                        synchronized(this)
                        {
                            _clients.remove(p);
                        }
                    }
                }
            }
        }
    }

    private com.zeroc.Ice.Communicator _communicator;
    private boolean _destroy = false;
    private java.util.List<CallbackReceiverPrx> _clients = new java.util.ArrayList<>();
}
