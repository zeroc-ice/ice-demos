// Copyright (c) ZeroC, Inc.

using Demo;

internal class HelloI(string serviceName) : HelloDisp_
{
    public override void sayHello(Ice.Current current)
    {
        var lang = System.Environment.GetEnvironmentVariable("LANG") != null ?
                                                             System.Environment.GetEnvironmentVariable("LANG") : "en";
        var greeting = "Hello, ";
        if (lang.Equals("fr"))
        {
            greeting = "Bonjour, ";
        }
        else if (lang.Equals("de"))
        {
            greeting = "Hallo, ";
        }
        else if (lang.Equals("es"))
        {
            greeting = "Hola, ";
        }
        else if (lang.Equals("it"))
        {
            greeting = "Ciao, ";
        }
        System.Console.Out.WriteLine(greeting + _serviceName);
    }

    private string _serviceName = serviceName;
}
