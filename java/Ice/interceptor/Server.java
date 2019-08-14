//
// Copyright (c) ZeroC, Inc. All rights reserved.
//

class Server
{
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
            communicator.getProperties().setProperty("Ice.Default.Package", "com.zeroc.demos.Ice.interceptor");
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
                // Create an object adapter for the authenticator.
                //
                com.zeroc.Ice.ObjectAdapter authenticatorAdapter = communicator.createObjectAdapter("Authenticator");
                AuthenticatorI authenticator = new AuthenticatorI();
                authenticatorAdapter.add(authenticator, com.zeroc.Ice.Util.stringToIdentity("authenticator"));
                authenticatorAdapter.activate();

                //
                // List of all the operations to require authorization for.
                //
                java.util.HashSet<String> securedOperations = new java.util.HashSet(java.util.Arrays.asList("setTemp"));
                //
                // Create an object adapter for the thermostat.
                //
                com.zeroc.Ice.ObjectAdapter thermostatAdapter = communicator.createObjectAdapter("Thermostat");
                ThermostatI thermostat = new ThermostatI();
                thermostatAdapter.add(new InterceptorI(thermostat, authenticator, securedOperations), com.zeroc.Ice.Util.stringToIdentity("thermostat"));
                thermostatAdapter.activate();

                communicator.waitForShutdown();
            }
        }

        System.exit(status);
    }
}
