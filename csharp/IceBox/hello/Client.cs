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
        var twoway = HelloPrxHelper.checkedCast(
            communicator.propertyToProxy("Hello.Proxy").ice_twoway().ice_timeout(-1).ice_secure(false));
        if(twoway == null)
        {
            Console.Error.WriteLine("invalid proxy");
            return 1;
        }
        var oneway = HelloPrxHelper.uncheckedCast(twoway.ice_oneway());
        var batchOneway = HelloPrxHelper.uncheckedCast(twoway.ice_batchOneway());
        var datagram = HelloPrxHelper.uncheckedCast(twoway.ice_datagram());
        var batchDatagram = HelloPrxHelper.uncheckedCast(twoway.ice_batchDatagram());

        bool secure = false;

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
                    twoway.sayHello();
                }
                else if(line.Equals("o"))
                {
                    oneway.sayHello();
                }
                else if(line.Equals("O"))
                {
                    batchOneway.sayHello();
                }
                else if(line.Equals("d"))
                {
                    if(secure)
                    {
                        Console.WriteLine("secure datagrams are not supported");
                    }
                    else
                    {
                        datagram.sayHello();
                    }
                }
                else if(line.Equals("D"))
                {
                    if(secure)
                    {
                        Console.WriteLine("secure datagrams are not supported");
                    }
                    else
                    {
                        batchDatagram.sayHello();
                    }
                }
                else if(line.Equals("f"))
                {
                    batchOneway.ice_flushBatchRequests();
                    if(!secure)
                    {
                        batchDatagram.ice_flushBatchRequests();
                    }
                }
                else if(line.Equals("S"))
                {
                    secure = !secure;

                    twoway = HelloPrxHelper.uncheckedCast(twoway.ice_secure(secure));
                    oneway = HelloPrxHelper.uncheckedCast(oneway.ice_secure(secure));
                    batchOneway = HelloPrxHelper.uncheckedCast(batchOneway.ice_secure(secure));
                    datagram = HelloPrxHelper.uncheckedCast(datagram.ice_secure(secure));
                    batchDatagram = HelloPrxHelper.uncheckedCast(batchDatagram.ice_secure(secure));

                    if(secure)
                    {
                        Console.WriteLine("secure mode is now on");
                    }
                    else
                    {
                        Console.WriteLine("secure mode is now off");
                    }
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
                    Console.WriteLine("unknown command `" + line + "'");
                    menu();
                }
            }
            catch(Exception ex)
            {
                Console.Error.WriteLine(ex);
            }
        }
        while (!line.Equals("x"));

        return 0;
    }

    private static void menu()
    {
        Console.Write(
            "usage:\n" +
            "t: send greeting as twoway\n" +
            "o: send greeting as oneway\n" +
            "O: send greeting as batch oneway\n" +
            "d: send greeting as datagram\n" +
            "D: send greeting as batch datagram\n" +
            "f: flush all batch requests\n" +
            "S: switch secure mode on/off\n" +
            "x: exit\n" +
            "?: help\n");
    }
}
