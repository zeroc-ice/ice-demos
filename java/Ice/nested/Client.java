//
// Copyright (c) ZeroC, Inc. All rights reserved.
//

import com.zeroc.demos.Ice.nested.Demo.*;

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
            communicator.getProperties().setProperty("Ice.Default.Package", "com.zeroc.demos.Ice.nested");

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
        NestedPrx nested = NestedPrx.checkedCast(communicator.propertyToProxy("Nested.Proxy"));
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

        com.zeroc.Ice.ObjectAdapter adapter = communicator.createObjectAdapter("Nested.Client");
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
}
