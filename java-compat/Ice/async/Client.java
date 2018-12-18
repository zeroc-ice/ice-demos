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

    public static class Callback_Hello_sayHelloI extends Callback_Hello_sayHello
    {
        @Override
        public void response()
        {
        }

        @Override
        public void exception(Ice.LocalException ex)
        {
            System.err.println("sayHello AMI call failed:");
            ex.printStackTrace();
        }

        @Override
        public void exception(Ice.UserException ex)
        {
            if(ex instanceof Demo.RequestCanceledException)
            {
                System.out.println("Demo.RequestCanceledException");
            }
            else
            {
                ex.printStackTrace();
            }
        }
    }

    private static int run(Ice.Communicator communicator)
    {
        HelloPrx hello = HelloPrxHelper.checkedCast(communicator.propertyToProxy("Hello.Proxy"));
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
                    hello.begin_sayHello(5000, new Callback_Hello_sayHelloI());
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
            catch(Ice.UserException ex)
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
            "i: send immediate greeting\n" +
            "d: send delayed greeting\n" +
            "s: shutdown the server\n" +
            "x: exit\n" +
            "?: help\n");
    }
}
