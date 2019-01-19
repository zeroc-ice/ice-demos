//
// Copyright (c) ZeroC, Inc. All rights reserved.
//

import com.zeroc.demos.Ice.properties.Demo.*;

public class Server
{
    //
    // The servant implements the Slice interface Demo::Props
    //
    static class PropsI implements Props
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

        synchronized void updated(java.util.Map<String, String> changes)
        {
            _changes = changes;
            _called = true;
            notify();
        }

        java.util.Map<String, String> _changes;
        private boolean _called;
    }

    public static void main(String[] args)
    {
        int status = 0;
        java.util.List<String> extraArgs = new java.util.ArrayList<String>();

        //
        // Try with resources block - communicator is automatically destroyed
        // at the end of this try block
        //
        try(com.zeroc.Ice.Communicator communicator = com.zeroc.Ice.Util.initialize(args, "config.server", extraArgs))
        {
            communicator.getProperties().setProperty("Ice.Default.Package", "com.zeroc.demos.Ice.properties");
            //
            // Install shutdown hook to (also) destroy communicator during JVM shutdown.
            // This ensures the communicator gets destroyed when the user interrupts the application with Ctrl-C.
            //
            Runtime.getRuntime().addShutdownHook(new Thread(() -> communicator.destroy()));

            if(!extraArgs.isEmpty())
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
                com.zeroc.Ice.Object obj = communicator.findAdminFacet("Properties");
                com.zeroc.Ice.NativePropertiesAdmin admin = (com.zeroc.Ice.NativePropertiesAdmin)obj;
                admin.addUpdateCallback(changes -> props.updated(changes));

                com.zeroc.Ice.ObjectAdapter adapter = communicator.createObjectAdapter("Props");
                adapter.add(props, com.zeroc.Ice.Util.stringToIdentity("props"));
                adapter.activate();

                communicator.waitForShutdown();
            }
        }

        System.exit(status);
    }
}
