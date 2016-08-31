// **********************************************************************
//
// Copyright (c) 2003-2016 ZeroC, Inc. All rights reserved.
//
// **********************************************************************

public class Server extends com.zeroc.Ice.Application
{
    //
    // The servant implements the Slice interface Demo::Props as well as the
    // native callback interface com.zeroc.Ice.PropertiesAdminUpdateCallback.
    //
    static class PropsI implements Demo.Props, com.zeroc.Ice.PropertiesAdminUpdateCallback
    {
        PropsI()
        {
            _called = false;
        }

        @Override
        public synchronized java.util.Map<String, String> getChanges(com.zeroc.Ice.Current current)
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
        public void shutdown(com.zeroc.Ice.Current current)
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

    @Override
    public int run(String[] args)
    {
        if(args.length > 0)
        {
            System.err.println(appName() + ": too many arguments");
            return 1;
        }

        PropsI props = new PropsI();

        //
        // Retrieve the PropertiesAdmin facet and register the servant as the update callback.
        //
        com.zeroc.Ice.Object obj = communicator().findAdminFacet("Properties");
        com.zeroc.Ice.NativePropertiesAdmin admin = (com.zeroc.Ice.NativePropertiesAdmin)obj;
        admin.addUpdateCallback(props);

        com.zeroc.Ice.ObjectAdapter adapter = communicator().createObjectAdapter("Props");
        adapter.add(props, com.zeroc.Ice.Util.stringToIdentity("props"));
        adapter.activate();
        communicator().waitForShutdown();
        return 0;
    }

    public static void main(String[] args)
    {
        Server app = new Server();
        int status = app.main("Server", args, "config.server");
        System.exit(status);
    }
}
