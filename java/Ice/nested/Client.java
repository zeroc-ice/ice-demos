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
            /*
             * For this demo we won't destroy the communicator since it has to
             * wait for any outstanding invocations to complete which may take
             * some time if the nesting level is exceeded.
             *
             try
             {
                 communicator().destroy();
             }
             catch(com.zeroc.Ice.LocalException ex)
             {
                 ex.printStackTrace();
             }
            */
        }
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

        NestedPrx nested = NestedPrx.checkedCast(communicator().propertyToProxy("Nested.Proxy"));
        if(nested == null)
        {
            System.err.println("invalid proxy");
            return 1;
        }

        //
        // Ensure the invocation times out if the nesting level is too
        // high and there are no more threads in the thread pool to
        // dispatch the call.
        //
        nested = nested.ice_invocationTimeout(5000);

        com.zeroc.Ice.ObjectAdapter adapter = communicator().createObjectAdapter("Nested.Client");
        NestedPrx self =
            NestedPrx.uncheckedCast(adapter.createProxy(com.zeroc.Ice.Util.stringToIdentity("nestedClient")));
        adapter.add(new NestedI(self), com.zeroc.Ice.Util.stringToIdentity("nestedClient"));
        adapter.activate();

        System.out.println("Note: The maximum nesting level is sz * 2, with sz being");
        System.out.println("the maximum number of threads in the server thread pool. If");
        System.out.println("you specify a value higher than that, the application will");
        System.out.println("block or timeout.");
        System.out.println();

        java.io.BufferedReader in = new java.io.BufferedReader(new java.io.InputStreamReader(System.in));

        String s = null;
        do
        {
            try
            {
                System.out.print("enter nesting level or 'x' for exit: ");
                System.out.flush();
                s = in.readLine();
                if(s == null)
                {
                    break;
                }
                int level = Integer.parseInt(s);
                if(level > 0)
                {
                    nested.nestedCall(level, self);
                }
            }
            catch(NumberFormatException ex)
            {
            }
            catch(java.io.IOException ex)
            {
                ex.printStackTrace();
            }
            catch(com.zeroc.Ice.LocalException ex)
            {
                ex.printStackTrace();
            }
        }
        while(!s.equals("x"));

        return 0;
    }

    public static void main(String[] args)
    {
        Client app = new Client();
        int status = app.main("Client", args, "config.client");
        System.exit(status);
    }
}
