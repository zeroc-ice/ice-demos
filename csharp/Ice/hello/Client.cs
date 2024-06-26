// Copyright (c) ZeroC, Inc.

using Demo;

internal class Client
{
    public static int Main(string[] args)
    {
        int status = 0;

        try
        {
            // The new communicator is automatically destroyed (disposed) at the end of the
            // using statement
            using var communicator = Ice.Util.initialize(ref args, "config.client");
            // The communicator initialization removes all Ice-related arguments from args
            if (args.Length > 0)
            {
                Console.Error.WriteLine("too many arguments");
                status = 1;
            }
            else
            {
                status = Run(communicator);
            }
        }
        catch (Exception ex)
        {
            Console.Error.WriteLine(ex);
            status = 1;
        }

        return status;
    }

    private static int Run(Ice.Communicator communicator)
    {
        HelloPrx twoway = HelloPrxHelper.checkedCast(
            communicator.propertyToProxy("Hello.Proxy").ice_twoway().ice_secure(false));
        if (twoway == null)
        {
            Console.Error.WriteLine("invalid proxy");
            return 1;
        }
        HelloPrx oneway = (HelloPrx)twoway.ice_oneway();
        HelloPrx batchOneway = (HelloPrx)twoway.ice_batchOneway();
        HelloPrx datagram = (HelloPrx)twoway.ice_datagram();
        HelloPrx batchDatagram = (HelloPrx)twoway.ice_batchDatagram();

        bool secure = false;
        int timeout = -1;
        int delay = 0;

        Menu();

        string line = null;
        do
        {
            try
            {
                Console.Out.Write("==> ");
                Console.Out.Flush();
                line = Console.In.ReadLine();
                if (line == null)
                {
                    break;
                }
                if (line.Equals("t"))
                {
                    twoway.sayHello(delay);
                }
                else if (line.Equals("o"))
                {
                    oneway.sayHello(delay);
                }
                else if (line.Equals("O"))
                {
                    batchOneway.sayHello(delay);
                }
                else if (line.Equals("d"))
                {
                    if (secure)
                    {
                        Console.WriteLine("secure datagrams are not supported");
                    }
                    else
                    {
                        datagram.sayHello(delay);
                    }
                }
                else if (line.Equals("D"))
                {
                    if (secure)
                    {
                        Console.WriteLine("secure datagrams are not supported");
                    }
                    else
                    {
                        batchDatagram.sayHello(delay);
                    }
                }
                else if (line.Equals("f"))
                {
                    batchOneway.ice_flushBatchRequests();
                    if (!secure)
                    {
                        batchDatagram.ice_flushBatchRequests();
                    }
                }
                else if (line.Equals("T"))
                {
                    if (timeout == -1)
                    {
                        timeout = 2000;
                    }
                    else
                    {
                        timeout = -1;
                    }

                    twoway = (HelloPrx)twoway.ice_invocationTimeout(timeout);
                    oneway = (HelloPrx)oneway.ice_invocationTimeout(timeout);
                    batchOneway = (HelloPrx)batchOneway.ice_invocationTimeout(timeout);

                    if (timeout == -1)
                    {
                        Console.WriteLine("timeout is now switched off");
                    }
                    else
                    {
                        Console.WriteLine("timeout is now set to 2000ms");
                    }
                }
                else if (line.Equals("P"))
                {
                    if (delay == 0)
                    {
                        delay = 2500;
                    }
                    else
                    {
                        delay = 0;
                    }

                    if (delay == 0)
                    {
                        Console.WriteLine("server delay is now deactivated");
                    }
                    else
                    {
                        Console.WriteLine("server delay is now set to 2500ms");
                    }
                }
                else if (line.Equals("S"))
                {
                    secure = !secure;

                    twoway = (HelloPrx)twoway.ice_secure(secure);
                    oneway = (HelloPrx)oneway.ice_secure(secure);
                    batchOneway = (HelloPrx)batchOneway.ice_secure(secure);
                    datagram = (HelloPrx)datagram.ice_secure(secure);
                    batchDatagram = (HelloPrx)batchDatagram.ice_secure(secure);

                    if (secure)
                    {
                        Console.WriteLine("secure mode is now on");
                    }
                    else
                    {
                        Console.WriteLine("secure mode is now off");
                    }
                }
                else if (line.Equals("s"))
                {
                    twoway.shutdown();
                }
                else if (line.Equals("x"))
                {
                    // Nothing to do
                }
                else if (line.Equals("?"))
                {
                    Menu();
                }
                else
                {
                    Console.WriteLine("unknown command `" + line + "'");
                    Menu();
                }
            }
            catch (System.Exception ex)
            {
                Console.Error.WriteLine(ex);
            }
        }
        while (!line.Equals("x"));

        return 0;
    }

    private static void Menu()
    {
        Console.Write(
            "usage:\n" +
            "t: send greeting as twoway\n" +
            "o: send greeting as oneway\n" +
            "O: send greeting as batch oneway\n" +
            "d: send greeting as datagram\n" +
            "D: send greeting as batch datagram\n" +
            "f: flush all batch requests\n" +
            "T: set a timeout\n" +
            "P: set a server delay\n" +
            "S: switch secure mode on/off\n" +
            "s: shutdown server\n" +
            "x: exit\n" +
            "?: help\n");
    }
}
