// **********************************************************************
//
// Copyright (c) 2003-2016 ZeroC, Inc. All rights reserved.
//
// **********************************************************************

import Demo.*;

public class Client extends com.zeroc.Ice.Application
{
    class ShutdownHook extends Thread
    {
        @Override
        public void run()
        {
            try
            {
                communicator().destroy();
            }
            catch(com.zeroc.Ice.LocalException ex)
            {
                ex.printStackTrace();
            }
        }
    }

    private static void menu()
    {
        System.out.println(
            "usage:\n" +
            "i: send immediate greeting\n" +
            "d: send delayed greeting\n" +
            "s: shutdown the server\n" +
            "x: exit\n" +
            "?: help\n");
    }

    @Override
    public int run(String[] args)
    {
        if(args.length > 0)
        {
            System.err.println(appName() + ": too many arguments");
            return 1;
        }

        //
        // Since this is an interactive demo we want to clear the
        // Application installed interrupt callback and install our
        // own shutdown hook.
        //
        setInterruptHook(new ShutdownHook());

        HelloPrx hello = HelloPrx.checkedCast(communicator().propertyToProxy("Hello.Proxy"));
        if(hello == null)
        {
            System.err.println("invalid proxy");
            return 1;
        }

        menu();

        java.io.BufferedReader in = new java.io.BufferedReader(new java.io.InputStreamReader(System.in));

        String line = null;
        do
        {
            try
            {
                System.out.print("==> ");
                System.out.flush();
                line = in.readLine();
                if(line == null)
                {
                    break;
                }
                if(line.equals("i"))
                {
                    hello.sayHello(0);
                }
                else if(line.equals("d"))
                {
                    hello.sayHelloAsync(5000).whenComplete((result, ex) ->
                        {
                            if(ex instanceof Demo.RequestCanceledException)
                            {
                                System.out.println("Demo.RequestCanceledException");
                            }
                            else
                            {
                                System.err.println("sayHello AMI call failed:");
                                ex.printStackTrace();
                            }
                        });
                }
                else if(line.equals("s"))
                {
                    hello.shutdown();
                }
                else if(line.equals("x"))
                {
                    // Nothing to do
                }
                else
                {
                    System.out.println("unknown command `" + line + "'");
                    menu();
                }
            }
            catch(java.io.IOException ex)
            {
                ex.printStackTrace();
            }
            catch(com.zeroc.Ice.UserException ex)
            {
                ex.printStackTrace();
            }
            catch(com.zeroc.Ice.LocalException ex)
            {
                ex.printStackTrace();
            }
        }
        while(!line.equals("x"));

        return 0;
    }

    public static void main(String[] args)
    {
        Client app = new Client();
        int status = app.main("Client", args, "config.client");
        System.exit(status);
    }
}
