// **********************************************************************
//
// Copyright (c) 2003-present ZeroC, Inc. All rights reserved.
//
// **********************************************************************

import Demo.*;

public class Client
{
    public static void main(String[] args)
    {
        int status = 0;
        Ice.StringSeqHolder argsHolder = new Ice.StringSeqHolder(args);

        //
        // Try with resources block - communicator is automatically destroyed
        // at the end of this try block
        //
        try(Ice.Communicator communicator = Ice.Util.initialize(argsHolder, "config.client"))
        {
            if(argsHolder.value.length > 0)
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

    private static int run(Ice.Communicator communicator)
    {
        ContextPrx proxy = ContextPrxHelper.checkedCast(communicator.propertyToProxy("Context.Proxy"));
        if(proxy == null)
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
                if(line.equals("1"))
                {
                    proxy.call();
                }
                else if(line.equals("2"))
                {
                    java.util.Map<String, String> ctx = new java.util.HashMap<String, String>();
                    ctx.put("type", "Explicit");
                    proxy.call(ctx);
                }
                else if(line.equals("3"))
                {
                    java.util.Map<String, String> ctx = new java.util.HashMap<String, String>();
                    ctx.put("type", "Per-Proxy");
                    ContextPrx proxy2 = ContextPrxHelper.uncheckedCast(proxy.ice_context(ctx));
                    proxy2.call();
                }
                else if(line.equals("4"))
                {
                    Ice.ImplicitContext ic = communicator.getImplicitContext();
                    java.util.Map<String, String> ctx = new java.util.HashMap<String, String>();
                    ctx.put("type", "Implicit");
                    ic.setContext(ctx);
                    proxy.call();
                    ic.setContext(new java.util.HashMap<String, String>());
                }
                else if(line.equals("s"))
                {
                    proxy.shutdown();
                }
                else if(line.equals("x"))
                {
                    // Nothing to do
                }
                else if(line.equals("?"))
                {
                    menu();
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
            catch(Ice.LocalException ex)
            {
                ex.printStackTrace();
            }
        }
        while(!line.equals("x"));

        return 0;
    }

    private static void menu()
    {
        System.out.println(
            "usage:\n" +
            "1: call with no request context\n" +
            "2: call with explicit request context\n" +
            "3: call with per-proxy request context\n" +
            "4: call with implicit request context\n" +
            "s: shutdown server\n" +
            "x: exit\n" +
            "?: help\n");
    }

}
