// **********************************************************************
//
// Copyright (c) 2003-present ZeroC, Inc. All rights reserved.
//
// **********************************************************************

import Demo.*;

class CallbackSenderI extends _CallbackSenderDisp implements java.lang.Runnable
{
    CallbackSenderI(Ice.Communicator communicator)
    {
        _communicator = communicator;
    }

    synchronized public void
    destroy()
    {
        System.out.println("destroying callback sender");
        _destroy = true;

        this.notify();
    }

    @Override
    synchronized public void
    addClient(CallbackReceiverPrx client, Ice.Current current)
    {
        System.out.println("adding client `" + Ice.Util.identityToString(client.ice_getIdentity()) + "'");
        _clients.add((CallbackReceiverPrx)client.ice_fixed(current.con));
    }

    @Override
    synchronized public void
    run()
    {
        int num = 0;
        while(!_destroy)
        {
            try
            {
                this.wait(2000);
            }
            catch(java.lang.InterruptedException ex)
            {
            }

            if(!_destroy && !_clients.isEmpty())
            {
                ++num;

                for(CallbackReceiverPrx p : _clients)
                {
                    final CallbackReceiverPrx prx = p;
                    try
                    {
                        p.begin_callback(num, new Callback_CallbackReceiver_callback() {
                                @Override
                                public void
                                response()
                                {
                                }

                                @Override
                                public void
                                exception(Ice.LocalException ex)
                                {
                                    removeClient(prx, ex);
                                }
                            });
                    }
                    catch(Ice.CommunicatorDestroyedException e)
                    {
                        return; // expected during shutdown
                    }
                }
            }
        }
    }

    synchronized private void
    removeClient(CallbackReceiverPrx client, Exception ex)
    {
        System.out.println("removing client `" + Ice.Util.identityToString(client.ice_getIdentity()) + "':");
        ex.printStackTrace();
        _clients.remove(client);
    }

    private Ice.Communicator _communicator;
    private boolean _destroy = false;
    private java.util.List<CallbackReceiverPrx> _clients = new java.util.ArrayList<CallbackReceiverPrx>();
}
