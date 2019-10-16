//
// Copyright (c) ZeroC, Inc. All rights reserved.
//

import com.zeroc.demos.Ice.bidir.Demo.*;
import java.util.concurrent.*;

class CallbackSenderI implements CallbackSender
{
    @Override
    synchronized public void addClient(CallbackReceiverPrx client, com.zeroc.Ice.Current current)
    {
        System.out.println("adding client `" + com.zeroc.Ice.Util.identityToString(client.ice_getIdentity()) + "'");
        _clients.add(client.ice_fixed(current.con));
    }

    public void destroy()
    {
        System.out.println("destroying callback sender");
        _executorService.shutdown();
        try
        {
            _executorService.awaitTermination(10, TimeUnit.SECONDS);
        }
        catch(InterruptedException interrupted)
        {
            // ignored
        }
    }

    public void start()
    {
        _executorService.scheduleAtFixedRate(() -> invokeCallback(), 2, 2, TimeUnit.SECONDS);
    }

    synchronized private void invokeCallback()
    {
        if(!_clients.isEmpty())
        {
            _num++;

            //
            // Invoke callback on all clients; it's safe to do it within the synchronization
            // because Ice guarantees these async invocations never block the calling thread.
            //
            // We use whenCompleteAsync instead of whenComplete to ensure that if the
            // call completes immediately we delay the removeClient until after the iteration.
            //
            for(CallbackReceiverPrx p : _clients)
            {
                try
                {
                    p.callbackAsync(_num).whenCompleteAsync(
                        (r, t) ->
                        {
                            if(t != null)
                            {
                                removeClient(p, t);
                            }
                        },
                        _executorService);
                }
                catch(com.zeroc.Ice.CommunicatorDestroyedException e)
                {
                    //
                    // We're shutting down
                    //
                    break;
                }
            }
        }
    }

    synchronized private void removeClient(CallbackReceiverPrx p, Throwable t)
    {
        if(_clients.remove(p))
        {
            System.err.println("removing client `" + com.zeroc.Ice.Util.identityToString(p.ice_getIdentity()) + "':");
            t.printStackTrace();
        }
    }

    private ScheduledExecutorService _executorService = Executors.newSingleThreadScheduledExecutor();
    private java.util.List<CallbackReceiverPrx> _clients = new java.util.ArrayList<>();
    private int _num = 0;
}
