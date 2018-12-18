// **********************************************************************
//
// Copyright (c) 2003-present ZeroC, Inc. All rights reserved.
//
// **********************************************************************

using Demo;
using System;

public class Client
{
    public static int Main(string[] args)
    {
        int status = 0;

        try
        {
            //
            // The new communicator is automatically destroyed (disposed) at the end of the
            // using statement
            //
            using(var communicator = Ice.Util.initialize(ref args, "config.client"))
            {
                //
                // The communicator initialization removes all Ice-related arguments from args
                //
                if(args.Length > 0)
                {
                    Console.Error.WriteLine("too many arguments");
                    status = 1;
                }
                else
                {
                    status = run(communicator);
                }
            }
        }
        catch(Exception ex)
        {
            Console.Error.WriteLine(ex);
            status = 1;
        }

        return status;
    }

    private static int run(Ice.Communicator communicator)
    {
        var sender = CallbackSenderPrxHelper.checkedCast(communicator.propertyToProxy("CallbackSender.Proxy").
                                                         ice_twoway().ice_timeout(-1).ice_secure(false));
        if(sender == null)
        {
            Console.Error.WriteLine("invalid proxy");
            return 1;
        }

        var adapter = communicator.createObjectAdapter("Callback.Client");
        adapter.add(new CallbackReceiverI(), Ice.Util.stringToIdentity("callbackReceiver"));
        adapter.activate();

        var receiver = CallbackReceiverPrxHelper.uncheckedCast(
            adapter.createProxy(Ice.Util.stringToIdentity("callbackReceiver")));

        menu();

        string line = null;
        do
        {
            try
            {
                Console.Out.Write("==> ");
                Console.Out.Flush();
                line = Console.In.ReadLine();
                if(line == null)
                {
                    break;
                }
                if(line.Equals("t"))
                {
                    sender.initiateCallback(receiver);
                }
                else if(line.Equals("s"))
                {
                    sender.shutdown();
                }
                else if(line.Equals("x"))
                {
                    // Nothing to do
                }
                else if(line.Equals("?"))
                {
                    menu();
                }
                else
                {
                    Console.Out.WriteLine("unknown command `" + line + "'");
                    menu();
                }
            }
            catch(Exception ex)
            {
                Console.Error.WriteLine(ex);
            }
        }
        while(!line.Equals("x"));

        return 0;
    }

    private static void menu()
    {
        Console.Out.Write("usage:\n"
                          + "t: send callback\n"
                          + "s: shutdown server\n"
                          + "x: exit\n"
                          + "?: help\n");
    }
}
