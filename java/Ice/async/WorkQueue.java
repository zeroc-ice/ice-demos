// **********************************************************************
//
// Copyright (c) 2003-2016 ZeroC, Inc. All rights reserved.
//
// **********************************************************************

import Demo.*;

public class WorkQueue extends Thread
{
    class FutureEntry
    {
        java.util.concurrent.CompletableFuture<Void> future;
        int delay;
    }

    @Override
    public synchronized void run()
    {
        while(!_done)
        {
            if(_futures.size() == 0)
            {
                try
                {
                    wait();
                }
                catch(java.lang.InterruptedException ex)
                {
                }
            }

            if(_futures.size() != 0)
            {
                //
                // Get next work item.
                //
                FutureEntry entry = _futures.getFirst();

                //
                // Wait for the amount of time indicated in delay to
                // emulate a process that takes a significant period of
                // time to complete.
                //
                try
                {
                    wait(entry.delay);
                }
                catch(java.lang.InterruptedException ex)
                {
                }

                if(!_done)
                {
                    //
                    // Print greeting and send response.
                    //
                    _futures.removeFirst();
                    System.err.println("Belated Hello World!");
                    entry.future.complete((Void)null);
                }
            }
        }

        //
        // Throw exception for any outstanding requests.
        //
        for(FutureEntry p : _futures)
        {
            p.future.completeExceptionally(new RequestCanceledException());
        }
    }

    public synchronized void add(java.util.concurrent.CompletableFuture<Void> future, int delay)
    {
        if(!_done)
        {
            //
            // Add the work item.
            //
            FutureEntry entry = new FutureEntry();
            entry.future = future;
            entry.delay = delay;

            if(_futures.size() == 0)
            {
                notify();
            }
            _futures.add(entry);
        }
        else
        {
            //
            // Destroyed, throw exception.
            //
            future.completeExceptionally(new RequestCanceledException());
        }
    }

    public synchronized void _destroy()                  // Thread.destroy is deprecated.
    {
        _done = true;
        notify();
    }

    private java.util.LinkedList<FutureEntry> _futures = new java.util.LinkedList<>();
    private boolean _done = false;
}
