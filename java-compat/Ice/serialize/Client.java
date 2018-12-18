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
        GreetPrx greet = GreetPrxHelper.checkedCast(communicator.propertyToProxy("Greet.Proxy"));
        if(greet == null) {

            System.err.println("invalid proxy");
            return 1;
        }

        MyGreeting greeting = new MyGreeting();
        greeting.text = "Hello there!";
        MyGreeting nullGreeting = null;

        boolean sendNull = false;

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
                if(line.equals("g"))
                {
                    if(sendNull)
                    {
                        greet.sendGreeting(nullGreeting);
                    }
                    else
                    {
                        greet.sendGreeting(greeting);
                    }
                }
                else if(line.equals("t"))
                {
                    sendNull = !sendNull;
                }
                else if(line.equals("s"))
                {
                    greet.shutdown();
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
            "g: send greeting\n" +
            "t: toggle null greeting\n" +
            "s: shutdown server\n" +
            "x: exit\n" +
            "?: help\n");
    }
}
