// Copyright (c) ZeroC, Inc.

using Demo;

public class Client
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
        string name;
        do
        {
            Console.Out.Write("Please enter your name ==> ");
            Console.Out.Flush();

            name = Console.In.ReadLine();
            if (name == null)
            {
                return 1;
            }
            name = name.Trim();
        }
        while (name.Length == 0);

        var basePrx = communicator.propertyToProxy("SessionFactory.Proxy");
        var factory = SessionFactoryPrxHelper.checkedCast(basePrx);
        if (factory == null)
        {
            Console.Error.WriteLine("invalid proxy");
            return 1;
        }

        var session = factory.create(name);

        var hellos = new List<HelloPrx>();

        Menu();

        bool destroy = true;
        bool shutdown = false;
        while (true)
        {
            Console.Out.Write("==> ");
            Console.Out.Flush();
            string line = Console.In.ReadLine();
            if (line == null)
            {
                break;
            }
            if (line.Length > 0 && char.IsDigit(line[0]))
            {
                int index = int.Parse(line);
                if (index < hellos.Count)
                {
                    var hello = hellos[index];
                    hello.sayHello();
                }
                else
                {
                    Console.Out.WriteLine("Index is too high. " + hellos.Count +
                                          " hello objects exist so far.\n" +
                                          "Use `c' to create a new hello object.");
                }
            }
            else if (line.Equals("c"))
            {
                hellos.Add(session.createHello());
                Console.Out.WriteLine("Created hello object " + (hellos.Count - 1));
            }
            else if (line.Equals("s"))
            {
                destroy = false;
                shutdown = true;
                break;
            }
            else if (line.Equals("x"))
            {
                break;
            }
            else if (line.Equals("t"))
            {
                destroy = false;
                break;
            }
            else if (line.Equals("?"))
            {
                Menu();
            }
            else
            {
                Console.Out.WriteLine("Unknown command `" + line + "'.");
                Menu();
            }
        }

        if (destroy)
        {
            session.destroy();
        }
        if (shutdown)
        {
            factory.shutdown();
        }
        return 0;
    }

    private static void Menu()
    {
        Console.Out.WriteLine(
            "usage:\n" +
            "c:     create a new per-client hello object\n" +
            "0-9:   send a greeting to a hello object\n" +
            "s:     shutdown the server and exit\n" +
            "x:     exit\n" +
            "t:     exit without destroying the session\n" +
            "?:     help\n");
    }
}
