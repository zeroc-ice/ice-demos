// **********************************************************************
//
// Copyright (c) 2003-2017 ZeroC, Inc. All rights reserved.
//
// **********************************************************************

import Demo.*;

public class Client extends com.zeroc.Ice.Application
{
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

    public Client()
    {
        // We let CTRL-C kill the client
        super(com.zeroc.Ice.SignalPolicy.NoSignalHandling);
    }

    @Override
    public int run(String[] args)
    {
        if(args.length > 0)
        {
            System.err.println(appName() + ": too many arguments");
            return 1;
        }

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

        com.zeroc.Ice.ObjectPrx base = communicator().propertyToProxy("SessionFactory.Proxy");
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

    public static void main(String[] args)
    {
        Client app = new Client();
        int status = app.main("Client", args, "config.client");
        System.exit(status);
    }
}
