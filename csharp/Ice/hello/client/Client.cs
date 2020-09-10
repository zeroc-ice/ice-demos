//
// Copyright (c) ZeroC, Inc. All rights reserved.
//

using Demo;
using System;
using System.Configuration;
using System.Threading;
using ZeroC.Ice;

try
{
    // The new communicator is automatically destroyed (disposed) at the end of the using statement
    using var communicator = new Communicator(ref args, ConfigurationManager.AppSettings);

    // The communicator initialization removes all Ice-related arguments from args
    if(args.Length > 0)
    {
        throw new ArgumentException("too many arguments");
    }

    IHelloPrx twoway =  communicator.GetPropertyAsProxy("Hello.Proxy", IHelloPrx.Factory);
    if (twoway == null)
    {
        Console.Error.WriteLine("invalid proxy");
        return 1;
    }

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
                var source = new CancellationTokenSource(TimeSpan.FromMilliseconds(timeout));
                twoway.SayHello(delay, cancel: source.Token);
            }
            else if (line.Equals("T"))
            {
                if (timeout == -1)
                {
                    timeout = 2000;
                    Console.WriteLine("timeout is now set to 2000ms");
                }
                else
                {
                    timeout = -1;
                    Console.WriteLine("timeout is now switched off");
                }
            }
            else if (line.Equals("P"))
            {
                if (delay == 0)
                {
                    delay = 2500;
                    Console.WriteLine("server delay is now set to 2500ms");
                }
                else
                {
                    delay = 0;
                    Console.WriteLine("server delay is now deactivated");
                }
            }
            else if (line.Equals("S"))
            {
                secure = !secure;

                twoway = twoway.Clone(preferNonSecure: !secure);
                Console.WriteLine(secure ? "secure mode is now on" : "secure mode is now off");
            }
            else if (line.Equals("s"))
            {
                twoway.Shutdown();
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
                Console.WriteLine($"unknown command `{line}'");
                Menu();
            }
        }
        catch (Exception ex)
        {
            Console.Error.WriteLine(ex);
        }
    }
    while (!line.Equals("x"));

}
catch(Exception ex)
{
    Console.Error.WriteLine(ex);
    return 1;
}
return 0;


static void Menu()
{
    Console.Write(
@"usage:
    t: send greeting as twoway
    T: set a timeout
    P: set a server delay
    S: switch secure mode on/off
    s: shutdown server
    x: exit
    ?: help
");
}

