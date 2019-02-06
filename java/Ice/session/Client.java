//
// Copyright (c) ZeroC, Inc. All rights reserved.
//

import com.zeroc.demos.Ice.session.Demo.*;

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
            communicator.getProperties().setProperty("Ice.Default.Package", "com.zeroc.demos.Ice.session");

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
        java.io.BufferedReader in = new java.io.BufferedReader(new java.io.InputStreamReader(System.in));
        String name;
        try
        {
            do
            {
                System.out.print("Please enter your name ==> ");
                System.out.flush();
                name = in.readLine().trim();
            }
            while(name.length() == 0);
        }
        catch(java.io.IOException ex)
        {
            ex.printStackTrace();
            return 1;
        }

        com.zeroc.Ice.ObjectPrx base = communicator.propertyToProxy("SessionFactory.Proxy");
        SessionFactoryPrx factory = SessionFactoryPrx.checkedCast(base);
        if(factory == null)
        {
            System.err.println("invalid proxy");
            return 1;
        }

        SessionPrx session = factory.create(name);

        java.util.ArrayList<HelloPrx> hellos = new java.util.ArrayList<>();

        menu();

        boolean destroy = true;
        boolean shutdown = false;
        while(true)
        {
            System.out.print("==> ");
            System.out.flush();
            String line = null;
            try
            {
                line = in.readLine();
            }
            catch(java.io.IOException ex)
            {
                ex.printStackTrace();
                return 1;
            }
            if(line == null)
            {
                break;
            }
            if(line.length() > 0 && Character.isDigit(line.charAt(0)))
            {
                int index;
                try
                {
                    index = Integer.parseInt(line);
                }
                catch(NumberFormatException e)
                {
                    menu();
                    continue;
                }
                if(index < hellos.size())
                {
                    HelloPrx hello = hellos.get(index);
                    hello.sayHello();
                }
                else
                {
                    System.out.println("Index is too high. " + hellos.size() + " exist so far. " +
                                       "Use 'c' to create a new hello object.");
                }
            }
            else if(line.equals("c"))
            {
                hellos.add(session.createHello());
                System.out.println("created hello object " + (hellos.size() - 1));
            }
            else if(line.equals("s"))
            {
                destroy = false;
                shutdown = true;
                break;
            }
            else if(line.equals("x"))
            {
                break;
            }
            else if(line.equals("t"))
            {
                destroy = false;
                break;
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

        if(destroy)
        {
            session.destroy();
        }

        if(shutdown)
        {
            factory.shutdown();
        }

        return 0;
    }

    private static void menu()
    {
        System.out.println(
            "usage:\n" +
            "c:     create a new per-client hello object\n" +
            "0-9:   send a greeting to a hello object\n" +
            "s:     shutdown the server and exit\n" +
            "x:     exit\n" +
            "t:     exit without destroying the session\n" +
            "?:     help\n");
    }
}
