// **********************************************************************
//
// Copyright (c) 2003-2016 ZeroC, Inc. All rights reserved.
//
// **********************************************************************

using Demo;
using System;
using System.Threading;
using System.Collections.Generic;

public class Server : Ice.Application
{
    //
    // The servant implements the Slice interface Demo::Props as well as the
    // native callback interface Ice.PropertiesAdminUpdateCallback.
    //
    class PropsI : PropsDisp_, Ice.PropertiesAdminUpdateCallback
    {
        public PropsI()
        {
            _called = false;
        }

        override public Dictionary<string, string> getChanges(Ice.Current current)
        {
            lock(this)
            {
                //
                // Make sure that we have received the property updates before we
                // return the results.
                //
                while(!_called)
                {
                    Monitor.Wait(this);
                }

                _called = false;
                return _changes;
            }
        }
        
        override public void shutdown(Ice.Current current)
        {
            current.adapter.getCommunicator().shutdown();
        }

        public void updated(Dictionary<string, string> changes)
        {
            lock(this)
            {
                _changes = changes;
                _called = true;
                Monitor.Pulse(this);
            }
        }

        Dictionary<string, string> _changes;
        private bool _called;
    }

    override public int run(string[] args)
    {
        if(args.Length > 0)
        {
            Console.Error.WriteLine(appName() + ": too many arguments");
            return 1;
        }

        var props = new PropsI();

        //
        // Retrieve the PropertiesAdmin facet and register the servant as the update callback.
        //
        var obj = communicator().findAdminFacet("Properties");
        var admin = (Ice.NativePropertiesAdmin)obj;
        admin.addUpdateCallback(props);

        var adapter = communicator().createObjectAdapter("Props");
        adapter.add(props, Ice.Util.stringToIdentity("props"));
        adapter.activate();
        communicator().waitForShutdown();
        return 0;
    }

    public static int Main(string[] args)
    {
        var app = new Server();
        return app.main(args, "config.server");
    }
}
