// Copyright (c) ZeroC, Inc. All rights reserved.

using Demo;
using System;
using System.Collections.Generic;
using System.Configuration;
using ZeroC.Ice;

// The new communicator is automatically destroyed (disposed) at the end of the using statement
using var communicator = new Communicator(ref args, ConfigurationManager.AppSettings);

// The communicator initialization removes all Ice-related arguments from args
if (args.Length > 0)
{
    throw new ArgumentException("too many arguments");
}

IPropertiesPrx props = communicator.GetPropertyAsProxy("Properties.Proxy", Demo.IPropertiesPrx.Factory) ??
    throw new ArgumentException("invalid proxy");

IPropertiesAdminPrx admin = communicator.GetPropertyAsProxy("Admin.Proxy", IPropertiesAdminPrx.Factory) ??
    throw new ArgumentException("invalid admin proxy");

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

string? line = null;
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
        if (line == "1" || line == "2")
        {
            Dictionary<string, string> dict = line == "1" ? batch1 : batch2;
            Console.Out.WriteLine("Sending:");
            foreach ((string key, string value) in dict)
            {
                if (key.StartsWith("Demo"))
                {
                    Console.Out.WriteLine($"  {key}={value}");
                }
            }
            Console.Out.WriteLine();

            admin.SetProperties(dict);

            Console.Out.WriteLine("Changes:");
            Dictionary<string, string> changes = props.GetChanges();
            if (changes.Count == 0)
            {
                Console.Out.WriteLine("  None.");
            }
            else
            {
                foreach ((string key, string value) in changes)
                {
                    Console.Out.Write($"  {key}");
                    if (value.Length == 0)
                    {
                        Console.Out.WriteLine(" was removed");
                    }
                    else
                    {
                        Console.Out.WriteLine($" is now {value}");
                    }
                }
            }
        }
        else if (line == "c")
        {
            Show(admin);
        }
        else if (line == "s")
        {
            props.Shutdown();
        }
        else if (line == "x")
        {
            // Nothing to do
        }
        else if (line == "?")
        {
            Menu();
        }
        else
        {
            Console.Out.WriteLine($"unknown command `{line}'");
            Menu();
        }
    }
    catch (Exception ex)
    {
        Console.Error.WriteLine(ex);
    }
}
while (line != "x");

static void Menu()
{
    Console.Out.WriteLine(@"
usage:
    1: set properties (batch 1)
    2: set properties (batch 2)
    c: show current properties
    s: shutdown server
    x: exit
    ?: help
");
}

static void Show(IPropertiesAdminPrx admin)
{
    Dictionary<string, string> props = admin.GetPropertiesForPrefix("Demo");
    Console.Out.WriteLine("Server's current settings:");
    foreach ((string key, string value) in props)
    {
        Console.Out.WriteLine($"  {key}={value}");
    }
}
