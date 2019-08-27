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
                //
                // Create an object adapter for the authenticator.
                // We use separate adapters to allow for fine-grained control of the endpoints;
                // Only secure endpoints for the authenticator, and non-secure endpoints for
                // the thermostat. This demo doesn't make use of this, but is implemented this
                // way regardless to illustrate best practices.
                //
                ObjectAdapter authenticatorAdapter = communicator.createObjectAdapter("Authenticator");
                AuthenticatorI authenticator = new AuthenticatorI();
                authenticatorAdapter.add(authenticator, com.zeroc.Ice.Util.stringToIdentity("authenticator"));
                authenticatorAdapter.activate();

                //
                // List of all the operations to require authorization for.
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
