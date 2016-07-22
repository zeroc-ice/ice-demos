// **********************************************************************
//
// Copyright (c) 2003-2016 ZeroC, Inc. All rights reserved.
//
// **********************************************************************

using System;
using System.Threading;
using System.Collections.Generic;
using Demo;

public class WorkQueue
{
    public class CallbackEntry
    {
        public CallbackEntry(Action response, Action<Exception> exception, int delay)
        {
            this.response = response;
            this.exception = exception;
            this.delay = delay;
        }

        public Action response;
        public Action<Exception> exception;
        public int delay;
    }

    public void Join()
    {
        thread_.Join();
    }

    public void Start()
    {
        thread_ = new Thread(new ThreadStart(Run));
        thread_.Start();
    }

    public void Run()
    {
        lock(this)
        {
            while(!_done)
            {
                if(_callbacks.Count == 0)
                {
                    Monitor.Wait(this);
                }

                if(_callbacks.Count != 0)
                {
                    //
                    // Get next work item.
                    //
                    var entry = _callbacks[0];

                    //
                    // Wait for the amount of time indicated in delay to
                    // emulate a process that takes a significant period of
                    // time to complete.
                    //
                    Monitor.Wait(this, entry.delay);

                    if(!_done)
                    {
                        //
                        // Print greeting and send response.
                        //
                        _callbacks.RemoveAt(0);
                        Console.Out.WriteLine("Belated Hello World!");
                        entry.response();
                    }
                }
            }

            foreach(var e in _callbacks)
            {
                e.exception(new RequestCanceledException());
            }
        }
    }

    public void Add(CallbackEntry cb)
    {
        lock(this)
        {
            if(!_done)
            {
                if(_callbacks.Count == 0)
                {
                    Monitor.Pulse(this);
                }
                _callbacks.Add(cb);
            }
            else
            {
                //
                // Destroyed, throw exception.
                //
                cb.exception(new RequestCanceledException());
            }
        }
    }

    public void destroy()
    {
        lock(this)
        {
            _done = true;
            Monitor.Pulse(this);
        }
    }

    private List<CallbackEntry> _callbacks = new List<CallbackEntry>();
    private bool _done = false;
    private Thread thread_;
}
