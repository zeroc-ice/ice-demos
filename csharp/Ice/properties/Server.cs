
// **********************************************************************
//
// Copyright (c) 2003-present ZeroC, Inc. All rights reserved.
//
// **********************************************************************

using Demo;
using System;
using System.Threading;
using System.Collections.Generic;

public class Server
{
    //
    // The servant implements the Slice interface Demo::Props as well as the
    // native callback interface Ice.PropertiesAdminUpdateCallback.
    //
    class PropsI : PropsDisp_
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

    public static int Main(string[] args)
    {
        int status = 0;

        try
        {
            //
            // using statement - communicator is automatically destroyed
            // at the end of this statement
            //
            using(var communicator = Ice.Util.initialize(ref args, "config.server"))
            {
                //
                // Destroy the communicator on Ctrl+C or Ctrl+Break
                //
                Console.CancelKeyPress += (sender, eventArgs) => communicator.destroy();

                if(args.Length > 0)
                {
                    Console.Error.WriteLine("too many arguments");
                    status = 1;
                }
                else
                {
                    var props = new PropsI();

                    //
                    // Retrieve the PropertiesAdmin facet and use props.updated as the update callback.
                    //
                    var obj = communicator.findAdminFacet("Properties");
                    var admin = (Ice.NativePropertiesAdmin)obj;
                    admin.addUpdateCallback(props.updated);

                    var adapter = communicator.createObjectAdapter("Props");
                    adapter.add(props, Ice.Util.stringToIdentity("props"));
                    adapter.activate();
                    communicator.waitForShutdown();
                }
            }
        }
        catch(Exception ex)
        {
            Console.Error.WriteLine(ex);
            status = 1;
        }

        return status;
    }
}
