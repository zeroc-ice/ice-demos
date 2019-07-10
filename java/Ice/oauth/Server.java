//
// Copyright (c) ZeroC, Inc. All rights reserved.
//

public class Server
{
    public static void main(String[] args)
    {
        int status = 0;
        java.util.List<String> extraArgs = new java.util.ArrayList<String>();

        OAuthProvider provider = null;
        Thread providerThread = null;

        //
        // Try with resources block - communicator is automatically destroyed
        // at the end of this try block
        //
        try(com.zeroc.Ice.Communicator communicator = com.zeroc.Ice.Util.initialize(args, "config.server", extraArgs))
        {
            communicator.getProperties().setProperty("Ice.Default.Package", "com.zeroc.demos.Ice.oauth");
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
                //
                // Start the OAuth provider server.
                //
                int port = Integer.parseInt(communicator.getProperties().getProperty("OAuth.Provider.Port"));
                provider = new OAuthProvider(port);
                providerThread = new Thread(provider);
                providerThread.start();
                System.out.println("OAuth provider started on port " + port);

                com.zeroc.Ice.ObjectAdapter adapter = communicator.createObjectAdapter("Thermostat");
                //
                // This demo uses a dispatch interceptor to determine whether clients have authorization
                // to invoke their requested operation before dispatching the request to the actual servant.
                //
                ThermostatI servant = new ThermostatI();
                adapter.add(new InterceptorI(servant, provider), com.zeroc.Ice.Util.stringToIdentity("thermostat"));
                adapter.activate();

                communicator.waitForShutdown();
            }
        }
        finally
        {
            try
            {
                //
                // Gracefully shutdown the provider server, and wait for it to finish.
                //
                if(provider != null)
                {
                    provider.shutdown();
                    providerThread.join();
                }
            }
            catch(InterruptedException ex){}
        }

        System.exit(status);
    }
}