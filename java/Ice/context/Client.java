//
// Copyright (c) ZeroC, Inc. All rights reserved.
//

import com.zeroc.demos.Ice.context.Demo.*;

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
            communicator.getProperties().setProperty("Ice.Default.Package", "com.zeroc.demos.Ice.context");
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
                status = run(communicator);
            }
        }

        System.exit(status);
    }

    private static int run(com.zeroc.Ice.Communicator communicator)
    {
        ContextPrx proxy = ContextPrx.checkedCast(communicator.propertyToProxy("Context.Proxy"));
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
                    java.util.Map<String, String> ctx = new java.util.HashMap<>();
                    ctx.put("type", "Per-Proxy");
                    ContextPrx proxy2 = proxy.ice_context(ctx);
                    proxy2.call();
                }
                else if(line.equals("4"))
                {
                    com.zeroc.Ice.ImplicitContext ic = communicator.getImplicitContext();
                    java.util.Map<String, String> ctx = new java.util.HashMap<>();
                    ctx.put("type", "Implicit");
                    ic.setContext(ctx);
                    proxy.call();
                    ic.setContext(new java.util.HashMap<>());
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
            catch(com.zeroc.Ice.LocalException ex)
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
