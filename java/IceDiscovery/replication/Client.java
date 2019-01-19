//
// Copyright (c) ZeroC, Inc. All rights reserved.
//

import com.zeroc.demos.IceDiscovery.replication.Demo.*;

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
            communicator.getProperties().setProperty("Ice.Default.Package", "com.zeroc.demos.IceDiscover.replication");

            if(!extraArgs.isEmpty())
            {
                System.err.println("too many arguments");
                status = 1;
            }
            else
            {
                status = run(communicator);
            }
        }

        System.exit(status);
    }

    private static int run(com.zeroc.Ice.Communicator communicator)
    {
        //
        // Get the hello proxy. We configure the proxy to not cache the
        // server connection with the proxy and to disable the locator
        // cache. With this configuration, the IceGrid locator will be
        // queried for each invocation on the proxy and the invocation
        // will be sent over the server connection matching the returned
        // endpoints.
        //
        com.zeroc.Ice.ObjectPrx obj = communicator.stringToProxy("hello");
        obj = obj.ice_connectionCached(false);
        obj = obj.ice_locatorCacheTimeout(0);

        HelloPrx hello = HelloPrx.checkedCast(obj);
        if(hello == null)
        {
            System.err.println("invalid proxy");
            return 1;
        }

        while(true)
        {
            System.out.print("enter the number of iterations: ");
            System.out.flush();

            try
            {
                java.io.BufferedReader in = new java.io.BufferedReader(new java.io.InputStreamReader(System.in));
                String line = in.readLine();
                if(line == null || line.equals("x"))
                {
                    break;
                }

                int count = Integer.parseInt(line);

                System.out.print("enter the delay between each greetings (in ms): ");
                System.out.flush();
                line = in.readLine();
                if(line == null || line.equals("x"))
                {
                    break;
                }
                int delay = Integer.parseInt(line);

                if(delay < 0)
                {
                    delay = 500; // 500 milli-seconds
                }

                for(int i = 0; i < count; i++)
                {
                    System.out.println(hello.getGreeting());
                    try
                    {
                        Thread.currentThread();
                        Thread.sleep(delay);
                    }
                    catch(InterruptedException ex1)
                    {
                    }
                }
            }
            catch(com.zeroc.Ice.LocalException ex)
            {
                ex.printStackTrace();
            }
            catch(NumberFormatException ex)
            {
                System.out.println("please specify a valid integer value");
            }
            catch(java.io.IOException ex)
            {
                ex.printStackTrace();
            }
        }

        return 0;
    }
}
