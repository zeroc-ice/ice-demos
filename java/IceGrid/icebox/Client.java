// **********************************************************************
//
// Copyright (c) 2003-2018 ZeroC, Inc. All rights reserved.
//
// **********************************************************************

import Demo.*;

public class Client
{
    public static void main(String[] args)
    {
        int status = 0;
        java.util.List<String> extraArgs = new java.util.ArrayList<>();

        //
        // Try with resources block - communicator is automatically destroyed
        // at the end of this try block
        //
        try(com.zeroc.Ice.Communicator communicator = com.zeroc.Ice.Util.initialize(args, "config.client", extraArgs))
        {
            //
            // Install shutdown hook for user interrupt like Ctrl-C
            //
            Runtime.getRuntime().addShutdownHook(new Thread(() ->
            {
                //
                // Destroy communicator to abandon ongoing remote calls
                // calling destroy multiple times is no-op
                //
                communicator.destroy();
            }));

            if(!extraArgs.isEmpty())
            {
                System.err.println("too many arguments");
                status = 1;
            }
            else
            {
                HelloPrx hello = HelloPrx.uncheckedCast(communicator.propertyToProxy("Hello.Proxy"));

                if(hello == null)
                {
                    System.err.println("Hello.Proxy not set");
                    status = 1;
                }
                else
                {
                    hello.sayHello();
                }
            }
        }

        System.exit(status);
    }
}
