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
        var props = PropsPrxHelper.checkedCast(communicator.propertyToProxy("Props.Proxy"));
        if (props == null)
        {
            Console.Error.WriteLine("invalid proxy");
            return 1;
        }

        var admin = Ice.PropertiesAdminPrxHelper.checkedCast(communicator.propertyToProxy("Admin.Proxy"));

        var batch1 = new Dictionary<string, string>()
        {
            { "Demo.Prop1", "1" },
            { "Demo.Prop2", "2" },
            { "Demo.Prop3", "3" }
        };

        var batch2 = new Dictionary<string, string>()
        {
            { "Demo.Prop1", "10" },
            { "Demo.Prop2", "" }, // An empty value removes this property
            { "Demo.Prop3", "30" }
        };

        Show(admin);
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
                if (line.Equals("1") || line.Equals("2"))
                {
                    var dict = line.Equals("1") ? batch1 : batch2;
                    Console.Out.WriteLine("Sending:");
                    foreach (var e in dict)
                    {
                        if (e.Key.StartsWith("Demo"))
                        {
                            Console.Out.WriteLine("  " + e.Key + "=" + e.Value);
                        }
                    }
                    Console.Out.WriteLine();

                    admin.setProperties(dict);

                    Console.Out.WriteLine("Changes:");
                    var changes = props.getChanges();
                    if (changes.Count == 0)
                    {
                        Console.Out.WriteLine("  None.");
                    }
                    else
                    {
                        foreach (var e in changes)
                        {
                            Console.Out.Write("  " + e.Key);
                            if (e.Value.Length == 0)
                            {
                                Console.Out.WriteLine(" was removed");
                            }
                            else
                            {
                                Console.Out.WriteLine(" is now " + e.Value);
                            }
                        }
                    }
                }
                else if (line.Equals("c"))
                {
                    Show(admin);
                }
                else if (line.Equals("s"))
                {
                    props.shutdown();
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
                    Console.Out.WriteLine("unknown command `" + line + "'");
                    Menu();
                }
            }
            catch (Exception ex)
            {
                Console.Error.WriteLine(ex);
            }
        }
        while (!line.Equals("x"));

        return 0;
    }

    private static void Menu()
    {
        Console.Out.WriteLine(
            "\n" +
            "usage:\n" +
            "1: set properties (batch 1)\n" +
            "2: set properties (batch 2)\n" +
            "c: show current properties\n" +
            "s: shutdown server\n" +
            "x: exit\n" +
            "?: help\n");
    }

    private static void Show(Ice.PropertiesAdminPrx admin)
    {
        var props = admin.getPropertiesForPrefix("Demo");
        Console.Out.WriteLine("Server's current settings:");
        foreach (KeyValuePair<string, string> e in props)
        {
            Console.Out.WriteLine("  " + e.Key + "=" + e.Value);
        }
    }
}
