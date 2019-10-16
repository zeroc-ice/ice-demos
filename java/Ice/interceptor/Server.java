//
// Copyright (c) ZeroC, Inc. All rights reserved.
//

import com.zeroc.Ice.Communicator;
import com.zeroc.Ice.ObjectAdapter;
import java.util.Arrays;
import java.util.ArrayList;
import java.util.HashSet;
import java.util.List;
import java.util.Set;

class Server
{
    public static void main(String[] args)
    {
        int status = 0;
        List<String> extraArgs = new ArrayList<String>();

        //
        // Try with resources block - communicator is automatically destroyed
        // at the end of this try block
        //
        try(Communicator communicator = com.zeroc.Ice.Util.initialize(args, "config.server", extraArgs))
        {
            communicator.getProperties().setProperty("Ice.Default.Package", "com.zeroc.demos.Ice.interceptor");
            //
            // Install shutdown hook to (also) destroy communicator during JVM shutdown.
            // This ensures the communicator gets destroyed when the user interrupts
            // the application with Ctrl-C.
            //
            Runtime.getRuntime().addShutdownHook(new Thread(() -> communicator.destroy()));

            if(!extraArgs.isEmpty())
            {
                System.err.println("too many arguments");
                status = 1;
            }
            else
            {
                // We use separate object adapters for the authenticator and the thermostat in
                // this demo, so each object has its own endpoints. This way accessibility can
                // be fined tuned on each object separately, through firewall rules for example.
                // It's implemented this way to illustrate best practices, even if the demo
                // doesn't make use of it for simplicity's sake.

                //
                // Create an object adapter for the authenticator.
                //
                ObjectAdapter authenticatorAdapter = communicator.createObjectAdapter("Authenticator");
                AuthenticatorI authenticator = new AuthenticatorI();
                authenticatorAdapter.add(authenticator, com.zeroc.Ice.Util.stringToIdentity("authenticator"));
                authenticatorAdapter.activate();

                //
                // Set of all the operations to require authorization for.
                //
                Set<String> securedOperations = new HashSet<>(Arrays.asList("setTemp", "shutdown"));
                //
                // Create an object adapter for the thermostat.
                //
                ObjectAdapter thermostatAdapter = communicator.createObjectAdapter("Thermostat");
                InterceptorI interceptor = new InterceptorI(new ThermostatI(), authenticator, securedOperations);
                thermostatAdapter.add(interceptor, com.zeroc.Ice.Util.stringToIdentity("thermostat"));
                thermostatAdapter.activate();

                communicator.waitForShutdown();
            }
        }

        System.exit(status);
    }
}
