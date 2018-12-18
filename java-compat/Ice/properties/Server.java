// **********************************************************************
//
// Copyright (c) 2003-present ZeroC, Inc. All rights reserved.
//
// **********************************************************************

public class Server
{
    //
    // The servant implements the Slice interface Demo::Props as well as the
    // native callback interface Ice.PropertiesAdminUpdateCallback.
    //
    static class PropsI extends Demo._PropsDisp implements Ice.PropertiesAdminUpdateCallback
    {
        PropsI()
        {
            _called = false;
        }

        @Override
        public synchronized java.util.Map<String, String> getChanges(Ice.Current current)
        {
            //
            // Make sure that we have received the property updates before we
            // return the results.
            //
            while(!_called)
            {
                try
                {
                    wait();
                }
                catch(InterruptedException ex)
                {
                }
            }

            _called = false;
            return _changes;
        }

        @Override
        public void shutdown(Ice.Current current)
        {
            current.adapter.getCommunicator().shutdown();
        }

        @Override
        public synchronized void updated(java.util.Map<String, String> changes)
        {
            _changes = changes;
            _called = true;
            notify();
        }

        java.util.Map<String, String> _changes;
        private boolean _called;
    }

    static class ShutdownHook extends Thread
    {
        @Override
        public void
        run()
        {
            _communicator.destroy();
        }

        ShutdownHook(Ice.Communicator communicator)
        {
            _communicator = communicator;
        }

        private final Ice.Communicator _communicator;
    }

    public static void
    main(String[] args)
    {
        int status = 0;
        Ice.StringSeqHolder argsHolder = new Ice.StringSeqHolder(args);

        //
        // Try with resources block - communicator is automatically destroyed
        // at the end of this try block
        //
        try(Ice.Communicator communicator = Ice.Util.initialize(argsHolder, "config.server"))
        {
            //
            // Install shutdown hook to (also) destroy communicator during JVM shutdown.
            // This ensures the communicator gets destroyed when the user interrupts the application with Ctrl-C.
            //
            Runtime.getRuntime().addShutdownHook(new ShutdownHook(communicator));

            if(argsHolder.value.length > 0)
            {
                System.err.println("too many arguments");
                status = 1;
            }
            else
            {
                PropsI props = new PropsI();

                //
                // Retrieve the PropertiesAdmin facet and register the servant as the update callback.
                //
                Ice.Object obj = communicator.findAdminFacet("Properties");
                Ice.NativePropertiesAdmin admin = (Ice.NativePropertiesAdmin)obj;
                admin.addUpdateCallback(props);

                Ice.ObjectAdapter adapter = communicator.createObjectAdapter("Props");
                adapter.add(props, Ice.Util.stringToIdentity("props"));
                adapter.activate();

                communicator.waitForShutdown();
            }
        }

        System.exit(status);
    }
}
